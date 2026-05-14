#include <stdio.h>
#include <stdlib.h>

/*
 * Creating a Software Stack using Linked List:
 * Implementing Stack operations such as Push, Pop, Peek
 *
 */

// Define structure for linked list node:
typedef struct node
{
    int data;
    struct node* next;
}node;

// Define top : Initialize a NULL
node* top=NULL;

// Push Operation:
// - During each push operation, a new node will be created
// - Current top will be updated as new node
// - new node will point to previous node
void stack_push(int data)
{
    // Creating new node
    node* new_node= malloc(sizeof(node));

    // Pushing data
    new_node->data = data;

    //Link new node to previous node
    new_node->next = top;

    // Update top
    top= new_node;
}

// Pop Operation:
// - Remove current top 
// - Update top as previous node
int stack_pop()
{
    
    // Edge case: Handle stack underflow
    if(top==NULL)
    {
        printf("\nStack Underflow ");
        return 0;
    }
    int data= top->data;

    // Use a temp node to free memory
    node* temp= top;

    // Update top
    top= top->next;

    free(temp);

    return data;
}

// Peek Operation:
// - Return the top element
int stack_peek()
{
    // Edge case: Handle empty stack
    if(top==NULL)
    {
        printf("\nStack is Empty");
        return -1;
    }
    return top->data;
}

// Display stack elements:
void stack_display()
{
    // Edge case: Handle empty stack
    if(top==NULL)
    {
        printf("\nStack is Empty");
        return;
    }
    // Loop through nodes:
    node* current= top;
    printf("\n\n");
    while(current!=NULL)
    {
        printf("%d ->", current->data);

        current= current->next;
    }
    printf("NULL");
}
int main()
{
    // Push operation:
    stack_push(10);
    stack_push(20);
    stack_push(30);
    stack_push(40);
    stack_push(50);
    stack_push(60);

    // Peek operation:
    stack_peek();

    // Pop Operation:
    stack_pop();

    // Peek operation:
    printf("\nTop Element : %d", stack_peek());

    // Display stack:
    stack_display();

    return 0;
}