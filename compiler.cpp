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

	//forward declaration so that optimizations can call optimize again
	void optimize(instruction* instr);

	void optimizeDELETE(instruction* instr)
	{
		//deleting const or registered var
		if (instr->args[0].type == 'c' || instr->args[0].type == '$')
		{
			instr->function = getFunctions()[NONE_ID];
			instr->resize(0);
			optimize(instr);
		}
	}

	void optimizeSET(instruction* instr)
	{
		//Setting a variable equal to itself
		if (instr->args[0] == instr->args[1])
		{
			instr->function = getFunctions()[NONE_ID];
			instr->resize(0);
			//good practice when changing instruction type to try and optimize further (even if the optimization might not exist)
			optimize(instr); 
		}
		else if (instr->args[0].type == 'c') //setting const to value
		{
			instr->function = getFunctions()[NONE_ID];
			instr->resize(0);
			optimize(instr);
		}
	}

	void optimizeADD(instruction* instr)
	{

		if (instr->args[0].type == 'c' && instr->args[1].type == 'c') //adding two consts
		{
			instr->function = getFunctions()[SET_ID];
			*(AD_DEFAULT_TYPE*)(instr->args[1].data) = getConstVal(instr->args[0]) + getConstVal(instr->args[1]);
			std::swap(instr->args[0], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[0].type == 'c' && getConstVal(instr->args[0]) == 0) //adding 0
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[1].type == 'c' && getConstVal(instr->args[1]) == 0) //adding 0
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[1]);
			std::swap(instr->args[1], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[2].type == 'c') //setting const to value
		{
			instr->function = getFunctions()[NONE_ID];
			instr->resize(0);
			optimize(instr);
		}
	}

	void optimizeSUB(instruction* instr)
	{
		if (instr->args[0].type == 'c' && instr->args[1].type == 'c') //subtracting two consts
		{
			instr->function = getFunctions()[SET_ID];
			*(AD_DEFAULT_TYPE*)(instr->args[1].data) = getConstVal(instr->args[0]) - getConstVal(instr->args[1]);
			std::swap(instr->args[0], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[0].type == 'c' && getConstVal(instr->args[0]) == 0) //subtracting 0
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[1].type == 'c' && getConstVal(instr->args[1]) == 0) //subtracting 0
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[1]);
			std::swap(instr->args[1], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[2].type == 'c') //setting const to value
		{
			instr->function = getFunctions()[NONE_ID];
			instr->resize(0);
			optimize(instr);
		}
	}

	void optimizeMULT(instruction* instr)
	{
		if (instr->args[0].type == 'c' && instr->args[1].type == 'c') //multiplying two consts
		{
			instr->function = getFunctions()[SET_ID];
			*(AD_DEFAULT_TYPE*)(instr->args[1].data) = getConstVal(instr->args[0]) * getConstVal(instr->args[1]);
			std::swap(instr->args[0], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[0].type == 'c' && getConstVal(instr->args[0]) == 1) //multiplying 1
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[1].type == 'c' && getConstVal(instr->args[1]) == 1) //multiplying 1
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[1]);
			std::swap(instr->args[1], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[2].type == 'c') //setting const to value
		{
			instr->function = getFunctions()[NONE_ID];
			instr->resize(0);
			optimize(instr);
		}
	}

	void optimizeDIV(instruction* instr)
	{
		if (instr->args[0].type == 'c' && instr->args[1].type == 'c') //dividing two consts
		{
			instr->function = getFunctions()[SET_ID];
			*(AD_DEFAULT_TYPE*)(instr->args[1].data) = getConstVal(instr->args[0]) + getConstVal(instr->args[1]);
			std::swap(instr->args[0], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[0].type == 'c' && getConstVal(instr->args[0]) == 1) //dividing 1
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[1].type == 'c' && getConstVal(instr->args[1]) == 1) //dividing 1
		{
			instr->function = getFunctions()[SET_ID];
			std::swap(instr->args[0], instr->args[1]);
			std::swap(instr->args[1], instr->args[2]);
			instr->resize(2);
			optimize(instr);
		}
		else if (instr->args[2].type == 'c') //setting const to value
		{
			instr->function = getFunctions()[NONE_ID];
			instr->resize(0);
			optimize(instr);
		}
	}

	std::map<void(*)(program*, arg*), void(*)(instruction*)> optimizationTable = {
		{DELETE, optimizeDELETE},
		{SET, optimizeSET},
		{ADD, optimizeADD},
		{SUB, optimizeSUB},
		{MULT, optimizeMULT},
		{DIV, optimizeDIV},
	};

	void registerOptimization(void(*instr)(program*, arg*), void(*func)(instruction*))
	{
		optimizationTable.insert(std::pair<void(*)(program*, arg*), void(*)(instruction*)>(instr, func));
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
		std::map<arg, unsigned int> varAccessTable;
		std::map<arg, instruction*> initLocation;
		for (auto instr : *instructions) //find if variables are actually constants
		{
			for (unsigned int i = 0; i < instr->argCnt; i++)
			{
				if (instr->args[i].type == 'v')
				{
					if (instr->function == MARK || instr->function == JUMP || instr->function == CJUMP)
					{
						continue;
					}


					if (varAccessTable.count(instr->args[i]) == 0)
					{
						varAccessTable.insert(std::pair<arg, unsigned int>(instr->args[i], 0));
					}

					if (instr->function == VAR)
					{
						initLocation.insert(std::pair<arg, instruction*>(instr->args[i], instr));
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
						auto initLoc = initLocation.at(instr->args[i])->args[1].data;
						unsigned int size = strlen(initLoc) + 1;
						char* tmp = new char[size];
						strcpy_s(tmp, size, initLoc);
						std::swap(tmp, instr->args[i].data);
						delete[] tmp;
						
						if (instr->function == DELETE)
						{
							instr->args[i].type = 'c';
						}
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
				inits.second->resize(0);
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
		std::map<arg, unsigned int> markers;

		for (unsigned int i = 0; i < instructionCnt; i++)
		{
			if (instructions[i]->function == MARK)
			{
				markers.insert(std::pair<arg, unsigned int>(instructions[i]->args[0], i));
				arg tmp = arg('c', (AD_DEFAULT_TYPE&)i);
				std::swap(tmp, instructions[i]->args[0]);
			}
		}

		for (int i = 0; i < instructionCnt; i++)
		{
			if (instructions[i]->function == JUMP)
			{
				arg tmp = arg('c', (AD_DEFAULT_TYPE&)markers.at(instructions[i]->args[0]));
				std::swap(tmp, instructions[i]->args[0]);
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

					arg* args = new arg[split.size() - 1];

					for (unsigned int i = 1; i < split.size(); i++)
					{
						if (isNumeric(split[i]))
						{
							int val = std::stol(split[i]);
							args[i - 1] = arg('c', val);
						}
						else
						{
							args[i - 1] = arg('v', split[i]);
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