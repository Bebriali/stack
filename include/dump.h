#ifndef DUMP
#define DUMP

#include "stack.h"

#define DO_DUMP(stk) StackDump(stk, __FILE__, __func__, __LINE__)

int StackDump (const Stack* stk, const char* file, const char* func, const int line);

#endif
