#include "compiler.h"

#include "asstd.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace ADscript
{
	bool isNumeric(std::string str)
	{
		char chr = str[0];
		switch (chr)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return true;
		default:
			return false;
		}	
	}

	bool compStr(char* str1, char* str2)
	{
		char* chr = &str1[0];
		char* ochr = &str2[0];
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
		return true;
	}

	void optimizeSET(instruction* instr)
	{
		//Setting a variable equal to itself
		if (compStr(instr->args[0], instr->args[1]))
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete instr->args[1];
			delete[] instr->args;
		}
	}

	std::map<void(*)(program*, char**), void(*)(instruction*)> optimizationTable = {
		{SET, optimizeSET}
	};

	void registerOptimization(void(*instr)(program*, char**), void(*func)(instruction*))
	{
		optimizationTable.insert(std::pair<void(*)(program*, char**), void(*)(instruction*)>(instr, func));
	}

	void optimize(instruction* instr)
	{
		try
		{
			auto func = optimizationTable.at(instr->function);
			func(instr);
		}
		catch (...)
		{
			//This just means no optimization exists, not an error
		}
	}

	program compile(const char* filename)
	{
		auto& fTable = getFunctionTable();

		std::vector<instruction*> instructions;

		std::ifstream file(filename);
		if (file.is_open())
		{
			
			std::string line;
			while (std::getline(file, line))
			{
				std::vector<std::string> split;
				std::string subline;
				std::stringstream substrm(line);
				while (std::getline(substrm, subline, ' '))
				{
					split.push_back(subline);
				}
				
				try
				{
					std::pair<unsigned int, unsigned int> idncnt = fTable.at(split[0]);
					unsigned int iid = idncnt.first;

					if (split.size() - 1 != idncnt.second)
					{
						throw "Compiler Error: Invalid Argument Count, Excpected: " + std::to_string(idncnt.second) + " Found:" + std::to_string(split.size() - 1) + '\n';
					}

					char** args = new char* [split.size() - 1];

					for (unsigned int i = 1; i < split.size(); i++)
					{
						if (isNumeric(split[i]))
						{
							AD_DEFAULT_TYPE val = std::stol(split[i]);
							args[i - 1] = (char*)new AD_DEFAULT_TYPE(val);
						}
						else
						{
							char* writable = new char[split[i].size() + 1];
							std::copy(split[i].begin(), split[i].end(), writable);
							writable[split[i].size()] = '\0';
							args[i - 1] = writable;
						}
					}

					instructions.push_back(new instruction{ getFunctions()[iid], unsigned int(split.size() - 1), args});
					optimize(instructions.back());
				}
				catch (...)
				{
					throw "Compiler Error: Invalid Function Name " + split[0] + '\n';
				}
			}

			file.close();
			program opt;
			opt.instructions = new instruction*[instructions.size()];
			opt.instructionCnt = instructions.size();
			for (unsigned int i = 0; i < opt.instructionCnt; i++)
			{
				opt.instructions[i] = instructions[i];
			}
			
			return opt;
		}
		throw "Compiler Error: Could Not Open File\n";
	}
}