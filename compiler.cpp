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

		return (*ochr == '\0');
	}

	//forward declaration so that optimizations can call optimize again
	void optimize(instruction* instr);

	void optimizeSET(instruction* instr)
	{
		//Setting a variable equal to itself
		if (compStr(instr->args[0], instr->args[1]))
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete instr->args[1];
			delete[] instr->args;
			instr->argCnt = 0;
			//good practice when changing instruction type to try and optimize further (even if the optimization might not exist)
			optimize(instr); 
		}
	}

	void optimizeADD(instruction* instr)
	{

		if (isConst(instr->args[0]) && isConst(instr->args[1])) //adding two consts
		{
			instr->function = getFunctions()[SET_ID];
			*(AD_DEFAULT_TYPE*)(instr->args[1] + 1) = getConstVal(instr->args[0]) + getConstVal(instr->args[1]);
			std::swap(instr->args[0], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
		else if (isConst(instr->args[0]) && getConstVal(instr->args[0]) == 0) //adding 0
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
		else if (isConst(instr->args[1]) && getConstVal(instr->args[1]) == 0) //adding 0
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[1]);
			std::swap(instr->args[1], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
	}

	void optimizeSUB(instruction* instr)
	{
		if (isConst(instr->args[0]) && isConst(instr->args[1])) //subtracting two consts
		{
			instr->function = getFunctions()[SET_ID];
			*(AD_DEFAULT_TYPE*)(instr->args[1] + 1) = getConstVal(instr->args[0]) - getConstVal(instr->args[1]);
			std::swap(instr->args[0], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
		else if (isConst(instr->args[0]) && getConstVal(instr->args[0]) == 0) //subtracting 0
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
		else if (isConst(instr->args[1]) && getConstVal(instr->args[1]) == 0) //subtracting 0
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[1]);
			std::swap(instr->args[1], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
	}

	void optimizeMULT(instruction* instr)
	{
		if (isConst(instr->args[0]) && isConst(instr->args[1])) //multiplying two consts
		{
			instr->function = getFunctions()[SET_ID];
			*(AD_DEFAULT_TYPE*)(instr->args[1] + 1) = getConstVal(instr->args[0]) * getConstVal(instr->args[1]);
			std::swap(instr->args[0], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
		else if (isConst(instr->args[0]) && getConstVal(instr->args[0]) == 1) //multiplying 1
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
		else if (isConst(instr->args[1]) && getConstVal(instr->args[1]) == 1) //multiplying 1
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[1]);
			std::swap(instr->args[1], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
	}

	void optimizeDIV(instruction* instr)
	{
		if (isConst(instr->args[0]) && isConst(instr->args[1])) //dividing two consts
		{
			instr->function = getFunctions()[SET_ID];
			*(AD_DEFAULT_TYPE*)(instr->args[1] + 1) = getConstVal(instr->args[0]) + getConstVal(instr->args[1]);
			std::swap(instr->args[0], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
		else if (isConst(instr->args[0]) && getConstVal(instr->args[0]) == 1) //dividing 1
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
		else if (isConst(instr->args[1]) && getConstVal(instr->args[1]) == 1) //dividing 1
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[1]);
			std::swap(instr->args[1], instr->args[2]);
			delete instr->args[2];
			instr->argCnt = 2;
			optimize(instr);
		}
	}

	void optimizeHOPBACK(instruction* instr)
	{
		if (isConst(instr->args[0]) && getConstVal(instr->args[0]) == 0) //moving nowhere
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete[] instr->args;
			instr->argCnt = 0;
			optimize(instr);
		}
	}

	void optimizeHOP(instruction* instr)
	{
		if (isConst(instr->args[0]) && getConstVal(instr->args[0]) == 0) //moving nowhere
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete[] instr->args;
			instr->argCnt = 0;
			optimize(instr);
		}
	}

	void optimizeCHOPBACK(instruction* instr)
	{
		if (isConst(instr->args[0]) && getConstVal(instr->args[0]) == 0) //moving nowhere
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete[] instr->args;
			instr->argCnt = 0;
			optimize(instr);
		}
	}

	void optimizeCHOP(instruction* instr)
	{
		if (isConst(instr->args[0]) && getConstVal(instr->args[0]) == 0) //moving nowhere
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete[] instr->args;
			instr->argCnt = 0;
			optimize(instr);
		}
	}

	std::map<void(*)(program*, char**), void(*)(instruction*)> optimizationTable = {
		{SET, optimizeSET},
		{ADD, optimizeADD},
		{SUB, optimizeSUB},
		{MULT, optimizeMULT},
		{DIV, optimizeDIV},
		{HOPBACK, optimizeHOPBACK},
		{HOP, optimizeHOP},
		{CHOPBACK, optimizeCHOPBACK},
		{CHOP, optimizeCHOP}
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
							char* mval = new char[sizeof(AD_DEFAULT_TYPE) + 1];
							char* cval = (char*)new AD_DEFAULT_TYPE(val);
							std::copy(cval, cval+sizeof(AD_DEFAULT_TYPE), mval + 1);
							delete cval;
							mval[0] = 'c'; //c for const!
							args[i - 1] = mval;
						}
						else
						{
							char* writable = new char[split[i].size() + 2];
							std::copy(split[i].begin(), split[i].end(), writable + 1);
							writable[split[i].size()+1] = '\0';
							writable[0] = 'v'; //v for var!
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