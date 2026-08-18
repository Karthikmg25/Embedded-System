
#ifndef STACK_H
#define STACK_H

#include <stdio.h>

#define STACK_SIZE 10

//define stack
typedef struct 
{
    int data[STACK_SIZE];
    int top;
    /* data */
}stack_t;
// stack APIs

/*
fn: initialize stack
*/
void stack_init(stack_t *s);
/*
fn: push - insert data at top
*/
void stack_push(stack_t *s, int data);
/*
fn: pop- remove data from the top
*/
int stack_pop(stack_t *s);
/*
fn: print - print stack elements
*/
void stack_print(stack_t *s);


#endif STACK_H