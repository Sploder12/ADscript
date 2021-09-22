#include "asstd.h"

#include <iostream>

namespace ADscript
{
	//can be assumed that a value is a variable otherwise
	inline bool isConst(char* str)
	{
		return (str[0] == 'c');
	}

	//getArgVal could be used instead but this is more direct
	AD_DEFAULT_TYPE getConstVal(char* str)
	{
		return *(AD_DEFAULT_TYPE*)(str + 1);
	}

	AD_DEFAULT_TYPE* getArgPtr(program* host, char* arg)
	{
		if (isConst(arg))
		{
			return (AD_DEFAULT_TYPE*)(arg+1);
		}
		else
		{
			if (arg[1] == '$')
			{
				auto& vTable = getVariableTable();

				return (AD_DEFAULT_TYPE*)vTable.at(arg + 2);
			}

			return (AD_DEFAULT_TYPE*)host->getVar(arg);
		}
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

	//takes 1 arg, a name to mark as a jump location
	//locations are set after optimizations
	void MARK(program* host, char** args) {}
	
	//takes 0 args, does nothing. Mainly exists for optimizations
	void NONE(program* host, char** args) {}
}