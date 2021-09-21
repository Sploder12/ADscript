#ifndef ASSTD_H
#define ASSTD_H

#include "asexe.h"

namespace ADscript
{
	void VAR(program* host, char** args);
	void POP(program* host, char** args);
	void DELETE(program* host, char** args);
	void END(program* host, char** args);

	void PRINT(program* host, char** args);

	void SET(program* host, char** args);
	void ADD(program* host, char** args);
	void SUB(program* host, char** args);
	void MULT(program* host, char** args);
	void DIV(program* host, char** args);

	void EQUAL(program* host, char** args);
	void NEQUAL(program* host, char** args);

	void JUMP(program* host, char** args);
	void CJUMP(program* host, char** args);
}

#endif