#ifndef ASEXE_H
#define ASEXE_H

#include <string>

namespace ADscript
{
	struct node
	{
		node(std::string ID, char* data);

		node* previous = nullptr;
		node* next = nullptr;

		std::string ID;
		char* data;

		~node();
	};

	struct linkedStack
	{
		node* top = nullptr;
		node* bottom = nullptr;

		void push(node* elem);

		void pop();

		void remove(const char* id);
	};

	struct instruction
	{
		instruction() = default;

		instruction(unsigned int IID, unsigned int argCnt, char** args);

		unsigned int IID;
		unsigned int argCnt;
		
		//args stored as array of byte array
		//type safety is only checked loosely by compiler
		char** args; 

		~instruction();
	};

	struct program
	{
		unsigned int curInstruction = 0;
		bool comparisonRet = false;

		//program memory consists of a cstr ID and a byte array value
		//memory is handled for you, the value is newed into existence and deleted out of it
		//this linkd list is treated like a stack, dont worry about it.
		linkedStack programMemory;

		//array of instruction pointers
		unsigned int instructionCnt;
		instruction** instructions;

		program() = default;

		program(program&& other) noexcept; //move

		program(const program& other); //copy

		void dumpInstructions();

		char* getVar(std::string id);

		void push(std::string id, char* val);

		void pop();

		void run();

		~program();
	};
}
#endif