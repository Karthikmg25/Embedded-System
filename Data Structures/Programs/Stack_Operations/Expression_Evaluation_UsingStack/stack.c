



#include "stack.h"


/*< STACK OPERATIONS >*/

// Fn    : stack_push
// note  : Push new data into the stack
// param : pointer to stack, data

void stack_push(stack_t *s, int data)
{
    // Before pushing check if stack is full
    // if so, return
    if(s->top == MAX-1)// maximum index
    {
        printf(" STACK OVERFLOW");
        return;
    }
    // To push, increment the top and add data to the new index
    s->top++;
    s->data[s->top] = data;
}

// Fn     : stack_pop
// note   : Pop new data out of the stack
// param  : pointer to stack
// return : popped data

int stack_pop(stack_t *s)
{
    // first check if stack is empty
    if(s->top==-1)
    {
        printf(" STACK IS EMPTY");
        return -1;
    }

    // return top data and decrement top
    int d= s->data[s->top];
    s->top--;
    return d;
}

// Fn     : stack_peek
// note   : Retun the top element
// param  : pointer to stack
// return : top data

int stack_peek(stack_t *s)
{
    // first check if stack is empty
    if(s->top==-1)
    {
        printf(" STACK IS EMPTY");
        return -1;
    }

    // return top data 
    
    return s->data[s->top];
}

void stack_print(stack_t *s)
{
     // first check if stack is empty
    if(s->top==-1)
    {
        printf(" STACK IS EMPTY");
        return ;
    }
    // Print elements:
    int i=0;
    while(i<= s->top)
    {
        printf(" %d", s->data[i]);
        i++;
    }
}
