#include "asexe.h"

#include "tables.h"

#include <iostream>

namespace ADscript
{

	node::node(std::string ID, char* data):
		ID(ID), data(data) {}

	node::~node()
	{
		delete[] data;
	}

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

	instruction::instruction(unsigned int IID, unsigned int argCnt, char** args) :
		IID(IID), argCnt(argCnt), args(args) {}

	instruction::~instruction()
	{
		for (unsigned int i = 0; i < argCnt; i++)
		{
			delete[] args[i];
		}
		delete[] args;
	}

	void doInstruction(instruction* instr, program* host)
	{
		auto& funcs = getFunctions();
		auto func = funcs[instr->IID];
		func(host, instr->args);
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
			node* newNode = new node(top->ID, new char(*top->data));
			programMemory.push(newNode);
			top = top->next;
		}
	}

	void program::run()
	{
		while (curInstruction < instructionCnt)
		{
			doInstruction(instructions[curInstruction], this);
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
			instruction* instr = instructions[i];
			std::cout << "Instruction " << i << " Function:" << instr->IID << " with ArgCount:" << instr->argCnt << " Args: ";
			for (unsigned int j = 0; j < instr->argCnt; j++)
			{
				std::cout << instr->args[j] << " ";
			}
			std::cout << "\n";
		}
	}

	char* program::getVar(std::string id)
	{
		node* cur = programMemory.bottom;
		while (cur)
		{
			if (cur->ID == id)
			{
				return cur->data;
			}
			cur = cur->previous;
		}
		return nullptr;
	}

	void program::push(std::string id, char* val)
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
