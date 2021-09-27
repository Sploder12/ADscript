/*
* Title: ADscript
* Author: Sploder12
* Date: 9/19/21
* Description:
* 
* ADscript ("A Dumb" script) is a scripting language meant to be used in tandem with C/C++
* The language is based off of x86 assembly but is heavily bound to C/C++ and the host process.
* 
* The main features of this language is the ability to connect C/C++ functions and variables directly to it easily.
*/

#include "compiler.h"
#include <iostream>

void doTest(ADscript::program* host, ADscript::arg* args)
{
	std::cout << "We did it!";
}

int main()
{
	
	int myVar = 3;

	ADscript::registerVariable("cVar", (char*)&myVar);

	ADscript::registerFunction("TEST", 0, doTest);

	ADscript::program prgm = ADscript::compile("test.ads");

	prgm.dumpInstructions();

	prgm.decompile("OptimizedOpt.ads");

	for (unsigned int i = 0; i < 100000; i++)
	{
		std::cout << "factorial of " << myVar << ": ";
		prgm.run();
		std::cout << myVar << '\n';
		//factorial of 720 causes an overflow
	}
	
	
	return 0;
}