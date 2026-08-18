
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

// define a node: data,  pointer to next node
typedef struct node
{
    int data;
    struct node* next;
}node;

// APIs

/*
 fn: creates n nodes and returns head node
 */
node* List_CreateNodes(int n);
/*
 fn: Prints entire linked list
 */
void List_print(node *head);
/*
 fn: inserts a new node at beginning
 params: pointer to head, data to insert
 */
void List_InsertAtBeginning(node** head, int data);
/*
 fn: inserts a new node at end
 params: pointer to head, data to insert
 */
void List_InsertAtEnd(node **head, int data);
/*
 fn: remove a node from end
 params: pointer to head
 return: data in removed node
 */
int List_RemoveFromBeginning(node **head);
/*
 fn: remove a node from end
 params: pointer to head
 return: data in removed node
 */
int List_RemoveFromEnd(node **head);
/*
 fn    : insert a node at a position
 params: pointer to head, data to insert, position to insert
 */
void List_InsertAtPosition(node **head, int data, int pos);
/*
 fn: remove list
 params: pointer to head
 */
void List_Remove(node **head);
/*
 fn: reverse list
 params: pointer to head
 */
void List_Reverse(node** head);


#endif 