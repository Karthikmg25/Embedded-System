#include <stdio.h>
#include <stdlib.h>

/* Implementing Queue data structure using Linked list */

// Define structure for a node:
typedef struct node
{
    int data;
    struct node* next;
}node;

// Declare Pointers for front and rear
// initialize as NULL: Queue is empty
node* front=NULL;
node* rear=NULL;


/* Queue Operations */

// Enqueue Operation
// - Create new nodes as rear
// - link newnode with preveious node
void Enqueue(int data)
{
    // Create a new node:
    node* new_node= (node*)malloc(sizeof(node));
 
    // Edge case 1: New allocation fails
    if(new_node == NULL)
    {
        printf("\nAllocation failed");
        return;
    }

    new_node->data = data;
    new_node->next= NULL;


    // Edge case 2: Queue is empty
    // First node is inserted as front:
    if(front == NULL)
    {
       front = rear = new_node;
       return;
    }
    // Normal case: update rear
    rear->next =new_node;
    rear = new_node;

}
// Dequeue Operation:
// Remoning nodes from front 
void Dequeue()
{
    // Edge case 1: Queue is empty
    if(front == NULL)
    {
        printf("\nQueue is empty: No elements to be removed");
        return;
    }
    // Edge case 2: removing last element
    if(front == rear)
    {
        // Reset rear back to NULL
        // - don't leave rear as a dangling pointer
        rear = NULL;
    }

    // Normal case:
    // Normal case: Update front as next node
    // - free current front
    node* temp = front;
    front = front->next;
    free(temp);
}
// Peek operation:
// return data in front node:
int Peek()
{
    // Edge case: Queue is Empty
    if(front == NULL)
    {
        printf("\nQueue is empty");
        return -1;
    }
    return front->data;
}
void Display_Queue()
{
    node* current = front;

    // Edge case: Queue is Empty
    if(front == NULL)
    {
        printf("\nQueue is empty");
        return;
    }
    printf("\n");
    while(current!= NULL)
    {
       printf("%d-> ",current->data);
       current = current->next;
    }
    printf("NULL");
}

int main()
{
    Enqueue(20);
    Enqueue(40);
    Enqueue(70);
    Enqueue(120);
    Enqueue(150);

    Display_Queue();

    Dequeue();
    Dequeue();
    Dequeue();
    Dequeue();
    Dequeue();

    Display_Queue();

}