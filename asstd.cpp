#include "asstd.h"

#include "tables.h"

#include <iostream>

namespace ADscript
{
	AD_DEFAULT_TYPE* getArgPtr(program* host, char* arg)
	{
		char* ptr;

		if (arg[0] == '$')
		{
			auto& vTable = getVariableTable();

			ptr = vTable.at(arg + 1);
			return (AD_DEFAULT_TYPE*)ptr;
		}

		ptr = host->getVar(arg);
		if (ptr == nullptr)
		{
			return (AD_DEFAULT_TYPE*)arg;
		}
		return (AD_DEFAULT_TYPE*)ptr;
	}

	int getArgVal(program* host, char* arg)
	{
		return *getArgPtr(host, arg);
	}

	//takes 2 args, a varID and initial value
	//note that the varID has already been converted by the compiler to an index
	void VAR(program* host, char** args)
	{
		host->push(args[0], (char*)new AD_DEFAULT_TYPE(getArgVal(host, args[1])));
	}

	//takes no args, pops top value from stack
	void POP(program* host, char** args)
	{
		host->pop();
	}

	//takes 1 arg, named value to delete
	void DELETE(program* host, char** args)
	{
		host->programMemory.remove(args[0]);
	}

	//takes no args, stops program
	void END(program* host, char** args)
	{
		host->curInstruction = host->instructionCnt;
	}

	//takes 1 arg, something to print
	void PRINT(program* host, char** args)
	{
		std::cout << getArgVal(host, args[0]) << '\n';
	}

	//takes 2 args, value and place to store
	void SET(program* host, char** args)
	{
		*getArgPtr(host, args[0]) = getArgVal(host, args[1]);
	}

	//takes 3 args, two vals to add and place to store
	void ADD(program* host, char** args)
	{
		*getArgPtr(host, args[2]) = getArgVal(host, args[0]) + getArgVal(host, args[1]);
	}

	//takes 3 args, two vals to subtract and place to store
	void SUB(program* host, char** args)
	{
		*getArgPtr(host, args[2]) = getArgVal(host, args[0]) - getArgVal(host, args[1]);
	}

	//takes 3 args, two vals to multiply and place to store
	void MULT(program* host, char** args)
	{
		*getArgPtr(host, args[2]) = getArgVal(host, args[0]) * getArgVal(host, args[1]);
	}

	//takes 3 args, two vals to divide and place to store
	void DIV(program* host, char** args)
	{
		*getArgPtr(host, args[2]) = getArgVal(host, args[0]) / getArgVal(host, args[1]);
	}

	//takes 2 args, two vals to compare
	//result can be found in host.comparisonRet
	void EQUAL(program* host, char** args)
	{
		host->comparisonRet = getArgVal(host, args[0]) == getArgVal(host, args[1]);
	}

	//takes 2 args, two vals to compare
	//result can be found in host.comparisonRet
	void NEQUAL(program* host, char** args)
	{
		host->comparisonRet = getArgVal(host, args[0]) != getArgVal(host, args[1]);
	}

	//takes 1 args, an instruction to jump to
	void JUMP(program* host, char** args)
	{
		AD_DEFAULT_TYPE val = getArgVal(host, args[0]);
		if (val < host->instructionCnt)
		{
			host->curInstruction = val - 1; //we subtract 1 because it will be incremented right after this
		}
	}

	//takes 1 args, an instruction to jump to
	//uses the bool inside comparisonRet for conditional
	void CJUMP(program* host, char** args)
	{
		if (host->comparisonRet)
		{
			JUMP(host, args);
		}
	}

	//takes 1 arg, how many instructions to jump backwards
	void HOPBACK(program* host, char** args)
	{
		AD_DEFAULT_TYPE val = getArgVal(host, args[0]);
		host->curInstruction -= val;
	}

	//takes 1 arg, how many instructions to jump forwards
	void HOP(program* host, char** args)
	{
		AD_DEFAULT_TYPE val = getArgVal(host, args[0]);
		host->curInstruction += val;
	}

	//takes 1 arg, how many instructions to jump backwards
	//conditionally
	void CHOPBACK(program* host, char** args)
	{
		if (host->comparisonRet)
		{
			HOPBACK(host, args);
		}
	}

	//takes 1 arg, how many instructions to jump forwards
	//conditionally
	void CHOP(program* host, char** args)
	{
		if (host->comparisonRet)
		{
			HOP(host, args);
		}
	}
	
	//takes 0 args, does nothing. Mainly exists for optimizations
	void NONE(program* host, char** args)
	{

	}
}