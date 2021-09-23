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

	arg::arg(const char type, AD_DEFAULT_TYPE& data) :
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
	}
	*/

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

	node::node(const std::string ID, const arg data) noexcept:
		ID(ID), data(data) {}

	void linkedStack::push(node* elem)
	{
		if (bottom != nullptr)
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

		delete removed;
	}

	void linkedStack::remove(const char* id)
	{
		node* cur = bottom;
		while (cur)
		{
			if (cur->ID == id)
			{
				if (cur->previous != nullptr)
				{
					cur->previous->next = cur->next;
				}
				else
				{
					top = nullptr;
				}

				if (cur->next != nullptr)
				{
					cur->next->previous = cur->previous;
				}
				else
				{
					bottom = nullptr;
				}

				break;
			}
			cur = cur->previous;
		}
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
			node* newNode = new node(top->ID, top->data);
			programMemory.push(newNode);
			top = top->next;
		}
	}

	void program::run()
	{
		while (curInstruction < instructionCnt)
		{
			instructions[curInstruction]->exe(this);
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
			const instruction* instr = instructions[i];
			std::cout << "Instruction " << i << " Function:" << instr->function << " with ArgCount:" << instr->argCnt << " Args: ";
			for (unsigned int j = 0; j < instr->argCnt; j++)
			{
				std::cout << instr->args[j].type << instr->args[j].data << " ";
			}
			std::cout << "\n";
		}
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

					if (instr->args[j].type == 'c')
					{
						file << std::to_string(*(AD_DEFAULT_TYPE*)(instr->args[j].data));
					}
					else
					{
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

	char* program::getVar(const std::string id)
	{
		node* cur = programMemory.bottom;
		while (cur)
		{
			if (cur->ID == id)
			{
				return cur->data.data;
			}
			cur = cur->previous;
		}
		return nullptr;
	}

	void program::push(const std::string id, const arg val)
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
