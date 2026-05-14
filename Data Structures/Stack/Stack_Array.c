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

int main()
{
    // initialise stack
    stack_t stack = {.data={0}, .top= -1};

    // Push operation:

    stack_push(&stack, 10);
    stack_push(&stack, 20);
    stack_push(&stack, 30);
    stack_push(&stack, 40);

    // Peek operation:
    printf("\nTop Element : %d", stack_peek(&stack));

    // Pop operation:
    stack_pop(&stack);

    stack_print(&stack);

 
    return 0;
}