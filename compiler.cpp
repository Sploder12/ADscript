#include "compiler.h"

#include "asstd.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace ADscript
{
	inline bool isNumeric(char chr) 
	{
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
			const auto func = optimizationTable.at(instr->function);
			func(instr);
		}
		catch (...)
		{
			//This just means no optimization exists, not an error
		}
	}

	void examineConsts(std::vector<instruction*>* instructions)
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
						instr->args[i] = arg(initLocation.at(instr->args[i])->args[1]);
						if (instr->function == DELETE)
						{
							instr->args[i].type = 'c';
						}
						optimize(instr);
					}
				}
			}
		}

		instruction* tmp = instructions->back();
		while (tmp)
		{
			if ((tmp->function == END) ||
				(tmp->function == EQUAL) ||
				(tmp->function == NEQUAL) ||
				(tmp->function == VAR) ||
				(tmp->function == NONE))
			{
				tmp->function = getFunctions()[NONE_ID];
				tmp->resize(0);
				instructions->pop_back();
				tmp = instructions->back();
			}
			else
			{
				tmp = nullptr;
				break;
			}
		}

		for (auto& inits : initLocation) //remove VAR for constant value
		{
			if (varAccessTable.at(inits.first) <= 0)
			{
				inits.second->function = getFunctions()[NONE_ID];
				inits.second->resize(0);
			}
		}
	}

	void removeNone(std::vector<instruction*>* instructions)
	{
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
	}

	//this is the BIG BOY optimizer, it doesn't have customizable optimizations but it does have more meaningful optimizations
	//It is highly highly recommended that the peephole optimizations are performed prior to this.
	void fullOptimize(std::vector<instruction*>* instructions)
	{
		removeNone(instructions);

		examineConsts(instructions);

		instruction* prev = instructions->front();
		for (unsigned int i = 1; i < instructions->size(); i++) //remove adjacent conditionals
		{
			if ((prev->function == EQUAL || prev->function == NEQUAL) &&
				(instructions->at(i)->function == EQUAL || instructions->at(i)->function == NEQUAL))
			{
				prev->function = getFunctions()[NONE_ID];
				prev->resize(0);
			}
			prev = instructions->at(i);
		}

		for (unsigned int i = 0; i < instructions->size(); i++) //remove code that cant be accessed
		{
			if (instructions->at(i)->function == JUMP)
			{
				for (unsigned int j = i + 1; j < instructions->size(); j++)
				{
					if (instructions->at(j)->function == MARK)
					{
						if (instructions->at(i)->args[0] == instructions->at(j)->args[0])
						{
							instructions->at(i)->function = getFunctions()[NONE_ID];
							instructions->at(i)->resize(0);
						}
						break;
					}
					instructions->at(j)->function = getFunctions()[NONE_ID];
					instructions->at(j)->resize(0);
				}
			}
		}

		for (unsigned int i = 0; i < instructions->size(); i++) //remove marks with no jumps
		{
			if (instructions->at(i)->function == MARK)
			{
				for (unsigned int j = 0; j < instructions->size(); j++)
				{
					if ((instructions->at(j)->function == JUMP || instructions->at(j)->function == CJUMP) &&
						(instructions->at(i)->args[0] == instructions->at(j)->args[0]))
					{
						break;
					}
				}
				instructions->at(i)->function = NONE;
				instructions->at(i)->resize(0);
			}
		}

		removeNone(instructions);
	}

	//converts the marker into a line number
	void setMarkerLocations(const unsigned int instructionCnt, instruction** instructions)
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
		const auto& fTable = getFunctionTable();

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
					const std::pair<unsigned int, unsigned int> idncnt = fTable.at(split[0]);
					const unsigned int iid = idncnt.first;
					const size_t size = split.size() - 1;

					if (size != idncnt.second)
					{
						std::cout << "In file " << filename << '\n';
						std::cout << "ADCompiler Error: Invalid Argument Count, Excpected: " << idncnt.second << " Found: " << size << '\n';
						return program();
					}

					arg* args = new arg[size];

					for (unsigned int i = 0; i < size; i++)
					{
						const std::string& tmp = split[i + 1];
						if (isNumeric(tmp[0]))
						{
							int val = std::stol(tmp);
							args[i] = arg('c', val);
						}
						else
						{
							if (tmp[0] == '$')
							{
								args[i] = arg('$', tmp.substr(1, tmp.size() - 1));
							}
							else
							{
								args[i] = arg('v', tmp);
							}
						}
					}

					instructions.push_back(new instruction{ getFunctions()[iid], unsigned int(size), args});
					optimize(instructions.back());
				}
				catch (std::out_of_range err)
				{
					std::cout << "In file " << filename << '\n';
					std::cout << "ADCompiler Error: Invalid Function Name " << split[0] << " | " << err.what() << '\n';
					return program();
				}
				catch (...)
				{
					std::cout << "In file " << filename << '\n';
					std::cout << "ADCompiler Error: Unknown Error, Possible optimizer error " << instructions.back()->toStr() << '\n';
					return program();
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
		std::cout << "ADCompiler Error: Could Not Open File " << filename << '\n';
	}
}