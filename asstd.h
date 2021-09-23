#ifndef ASSTD_H
#define ASSTD_H

#include "asexe.h"

#include "tables.h"

namespace ADscript
{
	bool compStr(const char* str1, const char* str2);

	AD_DEFAULT_TYPE getConstVal(const arg& str);

	bool isSTDfunc(void(*func)(program*, arg*));

	void VAR(program* host, arg* args);
	void DELETE(program* host, arg* args);
	void END(program* host, arg* args);

	void PRINT(program* host, arg* args);

	void SET(program* host, arg* args);
	void ADD(program* host, arg* args);
	void SUB(program* host, arg* args);
	void MULT(program* host, arg* args);
	void DIV(program* host, arg* args);

	void EQUAL(program* host, arg* args);
	void NEQUAL(program* host, arg* args);

	void JUMP(program* host, arg* args);
	void CJUMP(program* host, arg* args);

	void MARK(program* host, arg* args);
	
	void NONE(program* host, arg* args);
}

#endif