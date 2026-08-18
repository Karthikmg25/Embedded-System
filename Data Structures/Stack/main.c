
#include "stack.h"
#include <stdio.h>

int main()
{
    //create a local stack
    stack_t stack;

    //initialise stack
    stack_init(&stack);

    // insert elements
    stack_push(&stack, 5);
    stack_push(&stack, 10);
    stack_push(&stack, 15);
    stack_push(&stack, 20);

    // print elements
    stack_print(&stack);

    // remove 2 elements
    stack_pop(&stack);
    stack_pop(&stack);
    

    // print elements
    stack_print(&stack);

    return 0;

}