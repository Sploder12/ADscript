#include "asstd.h"

#include <iostream>

namespace ADscript
{
	bool compStr(const char* str1, const char* str2)
	{
		const char* chr = &str1[0];
		const char* ochr = &str2[0];
		while (*chr != '\0')
		{
			if (*ochr == '\0')
			{
				return false;
			}
			else if (*chr == *ochr)
			{
				chr += 1;
				ochr += 1;
			}
			else
			{
				return false;
			}
		}

		return (*ochr == '\0');
	}

	//getArgVal could be used instead but this is more direct
	AD_DEFAULT_TYPE getConstVal(const arg& str)
	{
		return *(AD_DEFAULT_TYPE*)(str.data);
	}

	bool isSTDfunc(void(*func)(program*, arg*))
	{
		auto funcs = getSTDFunctions();
		for (auto fnc : funcs)
		{
			if (func == fnc)
			{
				return true;
			}
		}
		return false;
	}

	AD_DEFAULT_TYPE* getArgPtr(program* host, arg& arg)
	{
		if (arg.type == 'c')
		{
			return (AD_DEFAULT_TYPE*)(arg.data);
		}
		else
		{
			if (arg.type == '$')
			{
				auto& vTable = getVariableTable();

				return (AD_DEFAULT_TYPE*)vTable.at(arg.data)->data;
			}

			return (AD_DEFAULT_TYPE*)host->getVar(arg);
		}
	}

	int getArgVal(program* host, arg& arg)
	{
		return *getArgPtr(host, arg);
	}

	//takes 2 args, a varID and initial value
	//note that the varID has already been converted by the compiler to an index
	void VAR(program* host, arg* args)
	{
		host->push(args[0]);
	}

	//takes 1 arg, named value to delete
	void DELETE(program* host, arg* args)
	{
		host->programMemory.remove(args[0]);
	}

	//takes no args, stops program
	void END(program* host, arg* args)
	{
		host->curInstruction = host->instructionCnt;
	}

	//takes 1 arg, something to print
	void PRINT(program* host, arg* args)
	{
		std::cout << getArgVal(host, args[0]) << '\n';
	}

	//takes 2 args, value and place to store
	void SET(program* host, arg* args)
	{
		*getArgPtr(host, args[0]) = getArgVal(host, args[1]);
	}

	//takes 3 args, two vals to add and place to store
	void ADD(program* host, arg* args)
	{
		*getArgPtr(host, args[2]) = getArgVal(host, args[0]) + getArgVal(host, args[1]);
	}

	//takes 3 args, two vals to subtract and place to store
	void SUB(program* host, arg* args)
	{
		*getArgPtr(host, args[2]) = getArgVal(host, args[0]) - getArgVal(host, args[1]);
	}

	//takes 3 args, two vals to multiply and place to store
	void MULT(program* host, arg* args)
	{
		*getArgPtr(host, args[2]) = getArgVal(host, args[0]) * getArgVal(host, args[1]);
	}

	//takes 3 args, two vals to divide and place to store
	void DIV(program* host, arg* args)
	{
		*getArgPtr(host, args[2]) = getArgVal(host, args[0]) / getArgVal(host, args[1]);
	}

	//takes 2 args, two vals to compare
	//result can be found in host.comparisonRet
	void EQUAL(program* host, arg* args)
	{
		host->comparisonRet = getArgVal(host, args[0]) == getArgVal(host, args[1]);
	}

	//takes 2 args, two vals to compare
	//result can be found in host.comparisonRet
	void NEQUAL(program* host, arg* args)
	{
		host->comparisonRet = getArgVal(host, args[0]) != getArgVal(host, args[1]);
	}

	//takes 1 args, an instruction to jump to
	void JUMP(program* host, arg* args)
	{
		AD_DEFAULT_TYPE val = getArgVal(host, args[0]);
		if (val < host->instructionCnt)
		{
			host->curInstruction = val - 1; //we subtract 1 because it will be incremented right after this
		}
	}

	//takes 1 args, an instruction to jump to
	//uses the bool inside comparisonRet for conditional
	void CJUMP(program* host, arg* args)
	{
		if (host->comparisonRet)
		{
			JUMP(host, args);
		}
	}

	//takes 1 arg, a name to mark as a jump location
	//locations are set after optimizations
	void MARK(program* host, arg* args) 
	{
		//IMPORTANT, C++ COMPILER WILL REMOVE THIS FUNCTION IF IT DOES NOTHING WHICH BREAKS THE CODE
		//THIS DOESN'T ACTUALLY DO ANYTHING BUT ENSURES MARK EXISTS
		#ifdef NDEBUG
		args->type = 'c'; 
		#endif
	}
	
	//takes 0 args, does nothing. Mainly exists for optimizations
	void NONE(program* host, arg* args) {}
}