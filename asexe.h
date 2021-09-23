#ifndef ASEXE_H
#define ASEXE_H

#include <string>

#ifndef AD_DEFAULT_TYPE
#define AD_DEFAULT_TYPE int
#endif

namespace ADscript
{
	struct arg
	{
		char type;
		char* data;

		arg(const char type, char* data) noexcept;

		arg(const char type, const std::string data);

		arg(const char type, AD_DEFAULT_TYPE& data);

		//arg(char type, int data);

		arg() = default;

		arg(arg&& other) noexcept; //move

		arg(const arg& other); //copy

		void operator=(const arg& other);
		void operator=(arg&& other) noexcept;

		bool operator==(const arg& other) const;
		bool operator<(const arg& other) const;
		bool operator>(const arg& other) const;

		~arg();
	};

	struct node
	{
		node(const arg& data) noexcept;

		node* previous = nullptr;
		node* next = nullptr;

		arg data;
	};

	struct linkedStack
	{
		node* top = nullptr;
		node* bottom = nullptr;

		void push(node* elem);

		void pop();

		void remove(arg var);
	};

	struct program;

	struct instruction
	{
		instruction() = default;

		instruction(void(*function)(program*, arg*), const unsigned int argCnt, arg* args) noexcept;

		void(*function)(program*, arg*);
		unsigned int argCnt;
		
		//args stored as array of byte array
		//type safety is only checked loosely by compiler
		arg* args; 

		void resize(const unsigned int size);

		void exe(program* host);

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

		void decompile(const char* filename);

		char* getVar(const arg& var);

		void push(const arg& val);

		void pop();

		void run();

		~program();
	};
}
#endif