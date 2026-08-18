
#include "linked_list.h"

// APIs definition
int data=0;
/*
 fn: creates n nodes and returns head node
 */
node* List_CreateNodes(int n)
{
    node *newnode, *head, *currentnode;

    //allocate memory for head node
    currentnode = malloc(sizeof(node));
    //save it as head node
    head = currentnode;
    currentnode->data = data++;
    currentnode->next = NULL;

    for(int i=1;i<n;i++)
    {
        //allocate memory for each new node
        newnode = malloc(sizeof(node));
        //initialize node
        newnode->data = data++;
        newnode->next = NULL;
        //link newnode with previous node
        currentnode->next = newnode;
        //update current node
        currentnode = newnode;

    }
    return head;
}
/*
 fn: Prints entire linked list
 */
void List_print(node *head)
{
    //validate list
    if(head == NULL)
    {
        printf("\nEmpty List");
        return;
    }

    node *currentnode = head;
    printf("\n");
    while(currentnode != NULL)
    {
        printf(" %d->", currentnode->data);
        currentnode = currentnode->next;

    }
    printf("NULL");
} 
/*
 fn: inserts a new node at beginning
 params: pointer to head, data to insert
 */
void List_InsertAtBeginning(node** head, int data)
{
    
    //create a newnode
    node* newnode = malloc(sizeof(node));
    newnode->data = data;
    newnode->next = NULL;

    //link newnode with head node
    newnode->next = *head;

    //set newnode as new head
    *head = newnode;
    
}
/*
 fn: inserts a new node at end
 params: pointer to head, data to insert
 */
void List_InsertAtEnd(node **head, int data)
{
    node* currentnode = *head;

    //get the pointer to the last node
    while(currentnode->next != NULL)
    {
        currentnode = currentnode->next;
    }
    //create a newnode
    node* newnode = malloc(sizeof(node));
    newnode->data = data;
    newnode->next = NULL;// inserted at end, so points to NULL

    //link with last node
    currentnode->next = newnode;

}
/*
 fn: remove a node from end
 params: pointer to head
 return: data in removed node
 */
int List_RemoveFromBeginning(node **head)
{
    //validate list
    if(*head == NULL)
    {
        printf("\nEmpty List");
        return -1;
    }

    //store head in a temporary pointer
    node* temp = *head;
    int data = (*head)->data;
    //modify head as next node
    *head = (*head)->next;
    //free previous head
    free(temp); 

    return data;
}
/*
 fn: remove a node from end
 params: pointer to head
 return: data in removed node
 */
int List_RemoveFromEnd(node **head)
{
    //validate list
    if(*head == NULL)
    {
        printf("\nEmpty List");
        return -1;
    }
    node* currentnode = *head;
    //get pointer to secondlast node
    while(currentnode->next->next != NULL)
    {
        currentnode = currentnode->next;
    }
    int data = currentnode->next->data;//data in last node
    //free last node
    free(currentnode->next);
    //set as NULL
    currentnode->next = NULL;

    return data;
}
/*
 fn    : insert a node at a position
 params: pointer to head, data to insert, position to insert
 */
void List_InsertAtPosition(node **head, int data, int pos)
{
    //validate the pos value,? do it inside loop

    node* currentnode = *head;

    //get the pointer to the pos node
    int i=1;
    while(i<pos)
    {
        if(currentnode == NULL)
        {
            printf("\nInvalid position");
            return;
        }
        //after one iteration, currentnode points to 2nd node
        //after pos-1 iterations, points to pos node
        currentnode = currentnode->next;
        i++;
    }
    //create a newnode
    node* newnode = malloc(sizeof(node));
    newnode->data = data;
    newnode->next = NULL;// inserted at end, so points to NULL

    //link newnode with pos+1 node
    newnode->next = currentnode->next;

    //link newnode with pos node
    currentnode->next = newnode;

}
/*
 fn: remove list
 params: pointer to head
 */
void List_Remove(node **head)
{
    //validate list
    if(*head == NULL)
    {
        printf("\nEmpty List");
        return;
    }
    node* currentnode = *head, *temp;
    while(currentnode != NULL)
    {
        //free each nodes
        temp = currentnode;
        currentnode = currentnode->next;
        free(temp);
    }
    //set head as NULL
    *head = NULL;

}
/*
 fn: reverse list
 params: pointer to head
 */
void List_Reverse(node** head)
{
    // reversing: linking current node to previous node (opposite direction)
    //          : updating last node than as head.

    //validate list
    if(*head == NULL)
    {
        printf("\nEmpty List");
        return;
    }

    //3 temporary pointers are needed
    node* current=NULL, *next =NULL, *previous=NULL;

    current = *head;

    while(current != NULL)
    {
        //store the next node
        next = current->next;

        //link current with previous
        current->next = previous;

        //update both current and previous nodes
        previous = current;
        current  = next;

    }
    //update head as lastnode
    *head = previous; 
}
