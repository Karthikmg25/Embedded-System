#include <stdio.h>
#include "stack.h"

// stack APIs definition

/*
fn: initialize stack
*/
void stack_init(stack_t *s)
{
    //set top as -1: no elements
    s->top = -1;
}
/*
fn: push - insert data at top
*/
void stack_push(stack_t *s, int data)
{
    //check if stack is full
    if(s->top == STACK_SIZE-1)
    {
        printf("\nSTACK OVERFLOW");
        return;
    }
    // insert data at top
    s->top++;
    s->data[s->top] = data;

}
/*
fn: pop- remove data from the top
*/
int stack_pop(stack_t *s)
{
    //check if stack is empty
    if(s->top == -1)
    {
        printf("\nSTACK UNDERFLOW");
        return -1;
    }
    // remove data from top
    int top_data = s->data[s->top];

    // update top
    s->top--;

    return top_data;
}
/*
fn: print - print stack elements
*/
void stack_print(stack_t *s)
{
    printf("\n\n");
    //check if stack is empty
    if(s->top == -1)
    {
        printf("\nSTACK EMPTY");
        return ;
    }
    // print elements from top to bottom
    for(int i= s->top;i>=0;i--)
    {
        printf("\n %d", s->data[i]);
    }
}
