#include "asexe.h"

#include "tables.h"

#include <iostream>

namespace ADscript
{
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
		for (std::pair<std::string, char*> thing : other.programMemory)
		{
			thing.second = nullptr;
		}
		other.programMemory.clear();
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

		for (std::pair<std::string, char*> thing : other.programMemory)
		{
			programMemory.push_back(std::pair<std::string, char*>(thing.first, new char(*thing.second)));
		}
	}

	void program::run()
	{
		while (curInstruction < instructionCnt)
		{
			doInstruction(instructions[curInstruction], this);
			curInstruction++;
		}

		unsigned int siz = programMemory.size();
		for (unsigned int i = 0; i < siz; i++)
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
		for (int i = programMemory.size() - 1; i >= 0; i--)
		{
			if (programMemory[i].first == id)
			{
				return programMemory[i].second;
			}
		}
		return nullptr;
	}

	void program::push(std::string id, char* val)
	{
		programMemory.push_back({ id, val });
	}

	void program::pop()
	{
		delete[] programMemory.back().second;
		programMemory.pop_back();
	}

	program::~program()
	{
		for (unsigned int i = 0; i < instructionCnt; i++)
		{
			delete instructions[i];
		}
		delete[] instructions;

		unsigned int siz = programMemory.size();
		for (unsigned int i = 0; i < siz; i++)
		{
			pop();
		}
	}
}
