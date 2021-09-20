#include "tables.h"

#include "asstd.h"

namespace ADscript
{

	//table is formated as an identifier, {fID, arg count}
	//function takes the host program and the args as params. Host program is given so program vars table can be accessed
	std::map<std::string, std::pair<unsigned int, unsigned int>> functionTable =
	{
		{"VAR", {0, 2}},
		{"PRINT", {1, 1}},
		{"ADD", {2, 3}}
	};

	std::vector<void(*)(program*, char**)> functions =
	{
		VAR,
		PRINT,
		ADD
	};

	std::map<std::string, std::pair<unsigned int, unsigned int>>& getFunctionTable()
	{
		return functionTable;
	}

	std::vector<void(*)(program*, char**)>& getFunctions()
	{
		return functions;
	}

}