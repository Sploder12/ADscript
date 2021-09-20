#ifndef ASTABLES_H
#define ASTABLES_H

#include "asexe.h"

#include <map>
#include <string>


namespace ADscript
{
	
	std::map<std::string, std::pair<unsigned int, unsigned int>>& getFunctionTable();

	std::vector<void(*)(program*, char**)>& getFunctions();
}

#endif
