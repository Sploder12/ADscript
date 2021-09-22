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
		{"MARK", {MARK_ID, 1}},
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
		MARK,
		NONE
	};

	std::vector<void(*)(program*, char**)> STDfuncs = functions;

	std::map<std::string, std::pair<unsigned int, unsigned int>>& getFunctionTable()
	{
		return functionTable;
	}

	std::vector<void(*)(program*, char**)>& getFunctions()
	{
		return functions;
	}

	std::vector<void(*)(program*, char**)>& getSTDFunctions()
	{
		return STDfuncs;
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


	std::string getFunctionByPtr(void(*ipt)(program*, char**))
	{
		unsigned int fID = 0;
		for (unsigned int i = 0; i < functions.size(); i++)
		{
			if (functions[i] == ipt)
			{
				fID = i;
				break;
			}
		}

		for (auto ftval : functionTable)
		{
			if (ftval.second.first == fID)
			{
				return ftval.first;
			}
		}
		return "?";
	}
}