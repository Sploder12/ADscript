#include "asexe.h"

#include "tables.h"

#include <iostream>

namespace ADscript
{
	instruction::instruction(unsigned int IID, unsigned int argCnt, char** args) :
		IID(IID), argCnt(argCnt), args(args) {}

	void doInstruction(instruction* instr, program* host)
	{
		auto& funcs = getFunctions();
		auto func = funcs[instr->IID];
		func(host, instr->args);
	}

	void program::run()
	{
		while (curInstruction < instructionCnt)
		{
			doInstruction(instructions[curInstruction], this);
			curInstruction++;
		}
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

}
