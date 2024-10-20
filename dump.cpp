#include <stdio.h>
#include <assert.h>

#include "dump.h"

//#define CONSOLE

#ifdef CONSOLE
    #define stream stdout
    #define ON_FILE(arg) ;
#else
    #define ON_FILE(arg) arg

#endif

const size_t LINE = 10;


int StackDump (const Stack* stk, const char* file, const char* func, const int line)
{
    assert (stk != NULL);

    ON_FILE(FILE * stream = fopen("log.txt", "a"));

    for (size_t i = 0; i < LINE; i++)
    {
        fprintf(stream, "-");
    }

    fprintf(stream, "MY_STACK DUMP\n\n"
                    "called from\n"
                    "file: %s\n"
                    "func: %s\n"
                    "line: %d\n\n"
                    "capacity = %d\n"
                    "cur_size = %d\n",
                    file,
                    func,
                    line,
                    stk->capacity,
                    stk->cur_size);

    if (!stk->cur_size)
    {
        fprintf(stream, "no elements left in stack\n");
    }

    fprintf(stream, "LEFT CANARY = %lf\n", *(&stk->data[0] - 1));
    for (size_t i = 0;             i < stk->cur_size; i++)
    {
        fprintf(stream, "\tcell[%d] = %lf\n",        i, stk->data[i]);
    }
    for (size_t i = stk->cur_size; i < stk->capacity; i++)
    {
        fprintf(stream, "\tcell[%d] = %lf POIZON\n", i, stk->data[i]);
    }
    fprintf(stream, "RIGHT CANARY = %lf\n", stk->data[stk->capacity]);

    for (size_t i = 0; i < LINE; i++)
    {
        fprintf(stream, "-");
    }

    fprintf(stream, "\n");

    ON_FILE(fclose(stream));

    return 0;
}

