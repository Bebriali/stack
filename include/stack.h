#ifndef STACK
#define STACK

#include "debug_info.h"

typedef double StackCell_t ;

typedef StackCell_t Canary_t;

struct Stack
{
    StackCell_t* data;
    size_t cur_size;
    size_t capacity;
    ON_DEBUG(Canary_t left_can);
    ON_DEBUG(Canary_t right_can);
};

#endif
