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
			instr->argCnt = 0;
			//good practice when changing instruction type to try and optimize further (even if the optimization might not exist)
			optimize(instr); 
		}
		else if (isConst(instr->args[0])) //setting const to value
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete instr->args[1];
			instr->argCnt = 0;
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
		else if (isConst(instr->args[2])) //setting const to value
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete instr->args[1];
			delete instr->args[2];
			instr->argCnt = 0;
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
		else if (isConst(instr->args[2])) //setting const to value
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete instr->args[1];
			delete instr->args[2];
			instr->argCnt = 0;
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
		else if (isConst(instr->args[2])) //setting const to value
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete instr->args[1];
			delete instr->args[2];
			instr->argCnt = 0;
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
		else if (isConst(instr->args[2])) //setting const to value
		{
			instr->function = getFunctions()[NONE_ID];
			delete instr->args[0];
			delete instr->args[1];
			delete instr->args[2];
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
	};

	void registerOptimization(void(*instr)(program*, char**), void(*func)(instruction*))
	{
		optimizationTable.insert(std::pair<void(*)(program*, char**), void(*)(instruction*)>(instr, func));
	}

	//peephole optimizer, good for getting things ready for the big boy optimizer
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

	//this is the BIG BOY optimizer, it doesn't have customizable optimizations but it does have more meaningful optimizations
	//It is highly highly recommended that the peephole optimizations are performed prior to this.
	void fullOptimize(std::vector<instruction*>* instructions)
	{
		std::map<std::string, unsigned int> varAccessTable;
		std::map<std::string, instruction*> initLocation;
		for (auto instr : *instructions) //find if variables are actually constants
		{
			for (unsigned int i = 0; i < instr->argCnt; i++)
			{
				if (!isConst(instr->args[i]) && instr->args[i][1] != '$')
				{
					if (instr->function == MARK || instr->function == JUMP || instr->function == CJUMP)
					{
						continue;
					}


					if (varAccessTable.count(instr->args[i]) == 0)
					{
						varAccessTable.insert(std::pair <std::string, unsigned int>(instr->args[i], 0));
					}

					if (instr->function == VAR)
					{
						initLocation.insert(std::pair<std::string, instruction*>(instr->args[i], instr));
						continue;
					}
					
					if (
						(instr->function == SET && i == 0) ||
						(instr->function == ADD && i == 2) ||
						(instr->function == SUB && i == 2) ||
						(instr->function == MULT && i == 2) ||
						(instr->function == DIV && i == 2) ||
						(!isSTDfunc(instr->function)) //optimizer has no knowledge whether or not a custom function sets a value so we assume it does
						)
					{
						varAccessTable.at(instr->args[i]) += 1;
					}
				}
			}
		}

		for (auto instr : *instructions) //set all args that reference the const var to the const val
		{
			for (unsigned int i = 0; i < instr->argCnt; i++)
			{
				if (varAccessTable.count(instr->args[i]) == 1)
				{
					if (instr->function == VAR)
					{
						continue;
					}

					if (varAccessTable.at(instr->args[i]) <= 0)
					{
						unsigned int size = strlen(initLocation.at(instr->args[i])->args[1]) + 1;
						char* tmp = new char[size];
						strcpy_s(tmp, size, initLocation.at(instr->args[i])->args[1]);
						std::swap(tmp, instr->args[i]);
						delete[] tmp;

						optimize(instr);
					}
				}
			}
		}

		for (auto inits : initLocation) //remove VAR for constant value
		{
			if (varAccessTable.at(inits.first) <= 0)
			{
				inits.second->function = getFunctions()[NONE_ID];
				delete inits.second->args[0];
				delete inits.second->args[1];
				inits.second->argCnt = 0;
				optimize(inits.second);
			}
		}

		varAccessTable.clear();
		initLocation.clear();

		std::vector<instruction*> tmp;
		tmp.reserve(instructions->size());

		for (auto instr : *instructions) //remove NONE
		{
			if (instr->function != NONE)
			{
				tmp.push_back(instr);
			}
			else
			{
				delete instr;
			}
		}
		*instructions = tmp;
		tmp.clear();
		tmp.reserve(instructions->size());

		//@TODO more big boy optimizations


	}

	//converts the marker into a line number
	void setMarkerLocations(unsigned int instructionCnt, instruction** instructions)
	{
		std::map<std::string, unsigned int> markers;

		for (unsigned int i = 0; i < instructionCnt; i++)
		{
			if (instructions[i]->function == MARK)
			{
				markers.insert(std::pair<std::string, unsigned int>(instructions[i]->args[0], i));
				char* tmp = new char[sizeof(AD_DEFAULT_TYPE) + 1];
				*(int*)(tmp + 1) = (int)i;
				tmp[0] = 'c';
				std::swap(tmp, instructions[i]->args[0]);
				delete[] tmp;
			}
		}

		for (int i = 0; i < instructionCnt; i++)
		{
			if (instructions[i]->function == JUMP)
			{
				char* tmp = new char[sizeof(AD_DEFAULT_TYPE) + 1];
				*(int*)(tmp+1) = (int)markers.at(instructions[i]->args[0]);
				tmp[0] = 'c';
				std::swap(tmp, instructions[i]->args[0]);
				delete[] tmp;
			}
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
				if (line == "" || line[0] == ';')
				{
					continue;
				}

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

			fullOptimize(&instructions);

			program opt;
			opt.instructions = new instruction*[instructions.size()];
			opt.instructionCnt = instructions.size();
			for (unsigned int i = 0; i < opt.instructionCnt; i++)
			{
				opt.instructions[i] = instructions[i];
			}

			setMarkerLocations(opt.instructionCnt, opt.instructions);
			
			return opt;
		}
		throw "Compiler Error: Could Not Open File\n";
	}
}