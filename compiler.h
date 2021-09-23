#ifndef ASCOMPILER_H
#define ASCOMPILER_H

#include "tables.h"
#include "asexe.h"

namespace ADscript
{
	void registerOptimization(void(*instr)(program*, arg*), void(*func)(instruction*));

	program compile(const char* filename);
}

#endif