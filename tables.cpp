#include "tables.h"

#include "asstd.h"

namespace ADscript
{

	//table is formated as an identifier, {fID, arg count}
	//function takes the host program and the args as params. Host program is given so program vars table can be accessed
	std::map<std::string, std::pair<unsigned int, unsigned int>> functionTable =
	{
		{"VAR", {VAR_ID, 2}},
		{"POP", {POP_ID, 0}},
		{"DELETE", {DELETE_ID, 1}},
		{"END", {END_ID, 0}},
		{"PRINT", {PRINT_ID, 1}},
		{"SET", {SET_ID, 2}},
		{"ADD", {ADD_ID, 3}},
		{"SUB", {SUB_ID, 3}},
		{"MULT", {MULT_ID, 3}},
		{"DIV", {DIV_ID, 3}},
		{"EQUAL", {EQUAL_ID, 2}},
		{"NEQUAL", {NEQUAL_ID, 2}},
		{"JUMP", {JUMP_ID, 1}},
		{"CJUMP", {CJUMP_ID, 1}},
		{"HOPBACK", {HOPBACK_ID, 1}},
		{"HOP", {HOP_ID, 1}},
		{"CHOPBACK", {CHOPBACK_ID, 1}},
		{"CHOP", {CHOP_ID, 1}},
		{"NONE", {NONE_ID, 0}}
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
		CJUMP,
		HOPBACK,
		HOP,
		CHOPBACK,
		CHOP,
		NONE
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