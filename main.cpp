#include "TxLib.h"
#include <stdio.h>

#include "change_stack.h"
#include "debug_info.h"
#include "dump.h"
#include "error_keys.h"

int main(void)
{
    printf("size double = %d\n", sizeof(double));

    size_t capacity = 0;
    printf("enter stack capacity: ");
    scanf("%d", &capacity);

    struct Stack my_stack = {};
    ErrorKeys init_status = StackInit (&my_stack, capacity);
    if (init_status != NO_ERRORS)
    {
        printf("error in stack init\n");
    }
    else
    {
        printf("initialization succeeded\n");
    }

    int num = 0;
    printf("enter number of values: ");
    scanf("%d", &num);
    for (int i = 0; i < num; i++)
    {
        PutElem(&my_stack);
    }

//    StackDiv(&my_stack);

    DO_DUMP(&my_stack);

//    StackOut(&my_stack);

    printf("StackDtor returned status: %d\n", StackDtor(&my_stack));

    return 0;
}
