#include "tables.h"

#include "asstd.h"

namespace ADscript
{

	//table is formated as an identifier, {fID, arg count}
	//function takes the host program and the args as params. Host program is given so program vars table can be accessed
	std::map<std::string, std::pair<unsigned int, unsigned int>> functionTable =
	{
		{"VAR", {0, 2}},
		{"POP", {1, 0}},
		{"DELETE", {2, 1}},
		{"END", {3, 0}},
		{"PRINT", {4, 1}},
		{"SET", {5, 2}},
		{"ADD", {6, 3}},
		{"SUB", {7, 3}},
		{"MULT", {8, 3}},
		{"DIV", {9, 3}},
		{"EQUAL", {10, 2}},
		{"NEQUAL", {11, 2}},
		{"JUMP", {12, 1}},
		{"CJUMP", {13, 1}}
	};

	std::vector<void(*)(program*, char**)> functions =
	{
		VAR,
		POP,
		DELETE,
		END,
		PRINT,
		SET,
		ADD,
		SUB,
		MULT,
		DIV,
		EQUAL,
		NEQUAL,
		JUMP,
		CJUMP
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
		functionTable.insert({id, {unsigned int(functions.size()), argCount}});
		functions.push_back(func);
	}

	void registerVariable(std::string id, char* var)
	{
		variableTable.insert({ id, var });
	}
}