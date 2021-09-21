#ifndef ASSTD_H
#define ASSTD_H

#include "asexe.h"

#include "tables.h"

namespace ADscript
{
	bool isConst(char* str);
	AD_DEFAULT_TYPE getConstVal(char* str);

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

	void HOPBACK(program* host, char** args);
	void HOP(program* host, char** args);
	void CHOPBACK(program* host, char** args);
	void CHOP(program* host, char** args);
	
	void NONE(program* host, char** args);
}

#endif