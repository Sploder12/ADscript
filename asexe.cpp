#include "asexe.h"

#include "tables.h"
#include "asstd.h"

#include <iostream>
#include <fstream>

namespace ADscript
{
	arg::arg(const char type, char* data) noexcept:
		type(type), data(data)  {}

	arg::arg(const char type, const std::string data) :
		type(type)
	{
		this->data = new char[data.size() + 1];
		std::copy(data.begin(), data.end(), this->data);
		this->data[data.size()] = '\0';
	}

	arg::arg(const char type, AD_DEFAULT_TYPE data) :
		type(type)
	{
		this->data = new char[sizeof(AD_DEFAULT_TYPE)];
		char* cval = (char*)new AD_DEFAULT_TYPE(data);
		std::copy(cval, cval + sizeof(AD_DEFAULT_TYPE), this->data);
		delete (AD_DEFAULT_TYPE*)cval;
	}

	/*
	arg::arg(char type, int data) :
		type(type)
	{
		this->data = new char[sizeof(int)];
		char* cval = (char*)new int(data);
		std::copy(cval, cval + sizeof(int), this->data);
		delete (int*)cval;
	}*/

	arg::arg(arg&& other) noexcept :
		type(std::move(other.type)),
		data(std::move(other.data))
	{
		other.data = nullptr;
	}

	arg::arg(const arg& other) :
		type(other.type)
	{
		const size_t size = strlen(other.data);
		this->data = new char[size + 1];
		std::copy(other.data, other.data + size, this->data);
		data[size] = '\0';
	}

	void arg::operator=(const arg& other)
	{
		const size_t size = strlen(other.data);
		this->data = new char[size + 1];
		std::copy(other.data, other.data + size, this->data);
	}

	void arg::operator=(arg&& other) noexcept
	{
		this->type = std::move(other.type);
		this->data = std::move(other.data);
		other.data = nullptr;
	}

	bool arg::operator==(const arg& other) const
	{
		return (compStr(this->data, other.data) && this->type == other.type);
	}

	bool arg::operator<(const arg& other) const
	{
		std::string tmp = this->type + std::string(this->data);
		std::string tmp2 = other.type + std::string(other.data);
		return tmp < tmp2;
	}

	bool arg::operator>(const arg& other) const
	{
		std::string tmp = this->type + std::string(this->data);
		std::string tmp2 = other.type + std::string(other.data);
		return tmp > tmp2;
	}

	arg::~arg()
	{
		delete[] data;
	}

	node::node(const arg& id, char* data) noexcept:
		id(id), data(data) {}

	void linkedStack::push(node* elem)
	{
		if (bottom)
		{
			bottom->next = elem;
			elem->previous = bottom;
		}
			
		bottom = elem;

		if (top == nullptr)
		{
			top = elem;
		}
	}

	void linkedStack::pop()
	{
		node* removed = bottom;
		if (top == bottom)
		{
			top = nullptr;
			bottom = nullptr;
		}
		else
		{
			bottom = bottom->previous;
			bottom->next = nullptr;
		}

		delete[] removed->data;
		delete removed;
	}

	void linkedStack::remove(arg var)
	{
		node* cur = bottom;
		while (cur)
		{
			if (cur->id == var)
			{
				if (cur->previous)
				{
					cur->previous->next = cur->next;
					if (!cur->next)
					{
						bottom = cur->previous;
					}
				}

				if (cur->next)
				{
					cur->next->previous = cur->previous;
					if (!cur->previous)
					{
						top = cur->next;
					}
				}

				break;
			}
			cur = cur->previous;
		}
		delete[] cur->data;
		delete cur;
	}

	instruction::instruction(void(*function)(program*, arg*), const unsigned int argCnt, arg* args) noexcept:
		function(function), argCnt(argCnt), args(args) {}

	instruction::~instruction()
	{
		delete[] args;
	}

	void instruction::resize(const unsigned int size)
	{
		arg* tmp = new arg[size];
		for (unsigned int i = 0; i < std::min(size, argCnt); i++)
		{
			tmp[i].data = nullptr;
			std::swap(tmp[i], this->args[i]);
		}
		delete[] args;
		args = tmp;
		argCnt = size;
	}

	void instruction::exe(program* host)
	{
		function(host, this->args);
	}

	std::string instruction::toStr()
	{
		std::string opt = "Instruction:" + getFunctionByPtr(this->function) + " - ArgCnt: " + std::to_string(this->argCnt) + " - Args: ";
		for (unsigned int j = 0; j < this->argCnt; j++)
		{
			opt += this->args[j].type + std::string(this->args[j].data) + " ";
		}
		opt += "\n";
		return opt;
	}

	program::program(program&& other) noexcept
		:curInstruction(std::move(other.curInstruction)),
		programMemory(std::move(other.programMemory)),
		instructionCnt(std::move(other.instructionCnt)),
		instructions(std::move(other.instructions))
	{
		other.instructions = nullptr;
		other.instructionCnt = 0;
		other.programMemory.top = nullptr;
		other.programMemory.bottom = nullptr;
	}

	program::program(const program& other)
		:curInstruction(other.curInstruction),
		instructionCnt(other.instructionCnt)
	{
		instructions = new instruction* [instructionCnt];
		for (unsigned int i = 0; i < instructionCnt; i++)
		{
			instructions[i] = new instruction(*other.instructions[i]);
		}

		node* top = other.programMemory.top;
		while (top)
		{
			node* newNode = new node(*top);
			programMemory.push(newNode);
			top = top->next;
		}
	}

	void program::run()
	{
		while (curInstruction < instructionCnt)
		{
			instructions[curInstruction]->exe(this);
			#ifdef _DEBUG
			dumpStack(); //Good for debugging and flooding the console
			#endif
			curInstruction++;
		}

		while (programMemory.top)
		{
			pop();
		}

		curInstruction = 0;
	}

	void program::dumpInstructions()
	{
		for (unsigned int i = 0; i < instructionCnt; i++)
		{
			std::cout << instructions[i]->toStr();
		}
	}

	void program::dumpStack()
	{
		auto cur = programMemory.top;
		while (cur)
		{
			std::cout << "Arg " << cur->id.data << " with value " << *(AD_DEFAULT_TYPE*)cur->data << '\n';
			cur = cur->next;
		}
		std::cout << '\n';
	}

	void program::decompile(const char* filename)
	{
		std::ofstream file(filename);

		if (file.is_open())
		{
			for (unsigned int i = 0; i < instructionCnt; i++)
			{
				const instruction* instr = instructions[i];
				file << getFunctionByPtr(instr->function);
				for (unsigned int j = 0; j < instr->argCnt; j++)
				{
					file << ' ';

					switch (instr->args[j].type)
					{
					case 'c':
						file << std::to_string(*(AD_DEFAULT_TYPE*)(instr->args[j].data));
						break;
					case '$':
						file << '$'; //break is missing on purpose
					default:
						file << (instr->args[j].data);
					}
				}

				if (i != instructionCnt - 1)
				{
					file << "\n";
				}
			}
			file.close();
		}
	}

	char* program::getVar(const arg& var)
	{
		node* cur = programMemory.bottom;
		while (cur)
		{
			if (cur->id == var)
			{
				return cur->data;
			}
			cur = cur->previous;
		}
		return nullptr;
	}

	void program::push(const arg& id, char* val)
	{
		node* newNode = new node(id, val);
		programMemory.push(newNode);
	}

	void program::pop()
	{
		programMemory.pop();
	}

	program::~program()
	{
		for (unsigned int i = 0; i < instructionCnt; i++)
		{
			delete instructions[i];
		}
		delete[] instructions;

		while (programMemory.top)
		{
			pop();
		}
	}
}