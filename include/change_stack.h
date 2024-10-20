#ifndef CHANGE_STK
#define CHANGE_STK

#include "stack.h"
#include "error_keys.h"

enum CAPACITY
{
    CAP_STEP = 2,
    CAP_MIN  = 8
};

enum Side
{
    LESS = 0,
    MORE = 1
};


const double EPSILON = 1e-5;
const StackCell_t POIZON = 0.987654321;
const StackCell_t LEFT_CAN = (StackCell_t) 0xEDADE;
const StackCell_t RIGHT_CAN = (StackCell_t) 0xADEDA;

void StackAssert (Stack* stk, const char* file, const char* func, const int line);
ErrorKeys MyAssert (bool arg, const char* text_arg, ...);
int FloatEqual(double a, double b);

ErrorKeys StackInit (Stack* stack_init, size_t capacity);
int StackDtor (Stack* stk);

void PutElem (Stack* stk);
int StackPush (Stack* stk, StackCell_t value);
int StackPop (Stack* stk, StackCell_t* number);

void Resize (Stack* stk, bool side);
void MemAlloc (Stack* stk);

#endif
