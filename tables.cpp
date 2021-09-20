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

	std::map<std::string, char*> variableTable;

	std::map<std::string, char*>& getVariableTable()
	{
		return variableTable;
	}

	void registerFunction(std::string id, unsigned int argCount, void(*func)(program*, char**))
	{
		functionTable.insert({id, {functions.size(), argCount}});
		functions.push_back(func);
	}

	void registerVariable(std::string id, char* var)
	{
		variableTable.insert({ id, var });
	}
}