#ifndef ASTABLES_H
#define ASTABLES_H

#include "asexe.h"

#include <map>
#include <string>
#include <vector>

#define VAR_ID 0
#define DELETE_ID 1
#define END_ID 2
#define PRINT_ID 3
#define SET_ID 4
#define ADD_ID 5
#define SUB_ID 6
#define MULT_ID 7
#define DIV_ID 8
#define MOD_ID 9
#define EQUAL_ID 10
#define NEQUAL_ID 11
#define LESS_ID 12
#define GREAT_ID 13
#define JUMP_ID 14
#define CJUMP_ID 15
#define MARK_ID 16
#define NONE_ID 17

namespace ADscript
{
	const std::map<std::string, std::pair<unsigned int, unsigned int>>& getFunctionTable();

	const std::vector<void(*)(program*, arg*)>& getFunctions();

	const std::vector<void(*)(program*, arg*)>& getSTDFunctions();

	const std::map<std::string, arg*>& getVariableTable();

	void registerFunction(const std::string id, const unsigned int argCount, void(*func)(program*, arg*));

	void registerVariable(const std::string id, char* var);

	std::string getFunctionByPtr(void(*ipt)(program*, arg*));
}
#endif