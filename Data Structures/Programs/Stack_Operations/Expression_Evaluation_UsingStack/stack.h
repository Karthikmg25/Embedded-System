



#include <stdio.h>
#define MAX 40

// Create a software stack using array:
// structure contains an array to store data
// and pointer(index) to the latest(top) element 

typedef struct 
{
    int data[MAX];  /* data */
    int top;
  
}stack_t;

// Stack APIs

void stack_push(stack_t *s, int data);
int stack_pop(stack_t *s);
int stack_peek(stack_t *s);
void stack_print(stack_t *s);