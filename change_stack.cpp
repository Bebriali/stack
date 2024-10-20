#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string.h>

#include "change_stack.h"
#include "dump.h"

#define STK_ASSERT(arg) StackAssert(arg, __FILE__, __func__, __LINE__)

#define DO_ASSERT(arg, ...); {                                        \
                                a = MyAssert(arg, #arg, __VA_ARGS__); \
                                if (a != NO_ERRORS)                   \
                                {                                     \
                                    DO_DUMP(stk);                     \
                                }                                     \
                             }


int FloatEqual(double a, double b)
{
    if (a < b) return b - a < EPSILON;
    else       return a - b < EPSILON;
}

void PoizonFill (Stack* stk);

ErrorKeys StackInit (Stack* stack_init, size_t capacity)
{
    stack_init->data = (StackCell_t*) calloc(capacity + 2, sizeof(StackCell_t));
    if (stack_init->data == NULL)
    {
        printf("invalid memory allocation\n");
        return EX_ERRORS;
    }

    stack_init->data[0] = LEFT_CAN;
    stack_init->data[capacity + 1] = RIGHT_CAN;


    stack_init->data = (StackCell_t*)((char*)stack_init->data + sizeof(StackCell_t));
    printf("left canary  = %lf\n",  *(&stack_init->data[0] - 1));
    printf("right canary = %lf\n", *(&stack_init->data[capacity]));

    stack_init->cur_size = 0;
    stack_init->capacity = capacity;

    DO_DUMP(stack_init);

    PoizonFill(stack_init);

    DO_DUMP(stack_init);

    return NO_ERRORS;
}


int StackPush (Stack* stk, StackCell_t value)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    if (stk->cur_size + 1 == stk->capacity)
    {
        DO_DUMP(stk);
        Resize (stk, MORE);
        DO_DUMP(stk);
    }

    assert(stk->cur_size < stk->capacity);

    stk->data[stk->cur_size] = value;
    stk->cur_size += 1;
    assert(FloatEqual(stk->data[stk->cur_size - 1], value));

    printf("data[cur_size - 1] = %lg\n", stk->data[stk->cur_size - 1]);

    printf("cur_size = %d capacity = %d\n", stk->cur_size, stk->capacity);

    return 0;
}

void PutElem (Stack* stk)
{
    StackCell_t number = 0;

    printf("enter next elem: ");
    scanf("%lf", &number);

    StackPush (stk, number);
}

int StackPop (Stack* stk, StackCell_t* number)
{
    assert(stk != NULL);
    assert(number != NULL);
    STK_ASSERT(stk);

    if (stk->cur_size < stk->capacity / (CAP_STEP * CAP_STEP) && stk->cur_size > CAP_MIN)
    {
        Resize(stk, LESS);
    }

    *number = stk->data[--stk->cur_size];
    stk->data[stk->cur_size] = POIZON;

    return 0;
}

void Resize (Stack* stk, bool side)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    if (side == LESS)
        stk->capacity /= CAP_STEP;
    if (side == MORE)
        stk->capacity *= CAP_STEP;

//    else if (stk->cur_size + 1 == (stk->capacity / CAP_STEP) && CAP_STEP != stk->capacity)
//    {
//        printf("file %s line %d\ncapacity = %d\ncur_size = %d\nstep = %d\n",
//                __FILE__, __LINE__, stk->capacity, stk->cur_size, CAP_STEP);
//        stk->capacity /= CAP_STEP;
//    }

    MemAlloc(stk);

    STK_ASSERT(stk);
}

void MemAlloc(Stack* stk)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    void *tmp = realloc((StackCell_t*)((char*)stk->data - sizeof(StackCell_t)), (stk->capacity + 2) * sizeof(StackCell_t));
    if (tmp == NULL)
    {
        printf("invalid reallocation for stk->data: \ncapacity = %d\ncur_size = %d",
                                                 stk->capacity,stk->cur_size);
        assert(0);
    }

    stk->data = (StackCell_t*) ((char*)tmp + sizeof(StackCell_t));

    DO_DUMP(stk);
    PoizonFill (stk);
    DO_DUMP(stk);
}

void StackAssert (Stack* stk, const char* file, const char* func, const int line)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    ErrorKeys a = NO_ERRORS;

    DO_ASSERT(FloatEqual(*(StackCell_t*)((char*)stk->data - sizeof(StackCell_t)), LEFT_CAN), file, func, line);

    DO_ASSERT(stk->cur_size < stk->capacity, file, func, line);

    DO_ASSERT(FloatEqual(stk->data[stk->capacity], RIGHT_CAN), file, func, line);
}

ErrorKeys MyAssert (bool arg, const char* text_arg, ...)
{
    va_list latest;
    va_start(latest, text_arg);

    if (!arg)
    {
        printf("My Assertion failed:    file %s\n", va_arg(latest, char*));
        printf("                        func %s\n", va_arg(latest, char*));
        printf("                        line %d\n", va_arg(latest, int));
        printf("\t%s\n", text_arg);
        va_end(latest);
        return EX_ERRORS;
    }

    return NO_ERRORS;
}

void PoizonFill(Stack* stk)
{
    for (size_t i = stk->cur_size; i < stk->capacity; i++)
    {
        stk->data[i] = POIZON;
    }

    stk->data[stk->capacity] = RIGHT_CAN;

    STK_ASSERT(stk);
}

int StackDtor (Stack* stk)
{
    if (stk->data == NULL)
    {
        printf("Stack has already been deinited\n");
        return 0;
    }

    DO_DUMP(stk);

    assert(stk != NULL);
    assert(stk->data != NULL);

    printf("stk->data addr = %p\n", &stk->data[0]);

    free((StackCell_t*)((char*)stk->data - sizeof(StackCell_t)));

    stk->cur_size = 0;
    stk->capacity = 0;
    printf("stack deinited successfully\n");

    return 0;
}
