#include "asstd.h"

#include "tables.h"

#include <iostream>

namespace ADscript
{
	int* getArgPtr(program* host, char* arg)
	{
		char* ptr;

		if (arg[0] == '$')
		{
			auto& vTable = getVariableTable();

			ptr = vTable.at(arg + 1);
			return (int*)ptr;
		}

		ptr = host->getVar(arg);
		if (ptr == nullptr)
		{
			return (int*)arg;
		}
		return (int*)ptr;
	}

	int getArgVal(program* host, char* arg)
	{
		return *getArgPtr(host, arg);
	}

	//takes 2 args, a varID and initial value
	//note that the varID has already been converted by the compiler to an index
	void VAR(program* host, char** args)
	{
		host->push(args[0], (char*)new int(getArgVal(host, args[1])));
	}

	//takes 1 arg, something to print
	void PRINT(program* host, char** args)
	{
		std::cout << getArgVal(host, args[0]) << '\n';
	}

	//takes 3 args, two vals to add and place to store
	void ADD(program* host, char** args)
	{
		*getArgPtr(host, args[2]) = getArgVal(host, args[0]) + getArgVal(host, args[1]);
	}
}