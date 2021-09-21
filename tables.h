#ifndef ASTABLES_H
#define ASTABLES_H

#include "asexe.h"

#include <map>
#include <string>
#include <vector>

#ifndef AD_DEFAULT_TYPE
#define AD_DEFAULT_TYPE int
#endif

#define VAR_ID 0
#define POP_ID 1
#define DELETE_ID 2
#define END_ID 3
#define PRINT_ID 4
#define SET_ID 5
#define ADD_ID 6
#define SUB_ID 7
#define MULT_ID 8
#define DIV_ID 9
#define EQUAL_ID 10
#define NEQUAL_ID 11
#define JUMP_ID 12
#define CJUMP_ID 13
#define HOPBACK_ID 14
#define HOP_ID 15
#define CHOPBACK_ID 16
#define CHOP_ID 17
#define NONE_ID 18

namespace ADscript
{
	
	std::map<std::string, std::pair<unsigned int, unsigned int>>& getFunctionTable();

	std::vector<void(*)(program*, char**)>& getFunctions();

	std::map<std::string, char*>& getVariableTable();

	void registerFunction(std::string id, unsigned int argCount, void(*func)(program*, char**));

	void registerVariable(std::string id, char* var);
}

#endif
