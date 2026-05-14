#include <stdio.h>
#include <stdlib.h>


// Define a structure for node
typedef struct node
{
    int data;
    struct node* next;
}node;

// Define a macro to check allocation
#define CHECK_ALLOCATION(pnode)  do{\
                                    if(pnode==NULL)\
                                    {printf("\nERROR"); return;}\
                                    else printf("\nMEMORY ALLOCATED");\
                                   }while(0)
static int d=20;// accessible to all fns

// LINKED LIST OPERATIONS

void create_nodes(node **head, int n)// pass pointer to pointer to change the head pointer
{
    // Allocate next n nodes
    node* current= *head;
    int d=20;
    for(int i=0;i<n;i++)
    {
        node* newnode= malloc(sizeof(node));// new node created
        CHECK_ALLOCATION(newnode);
        newnode->data= d+=10;
        newnode->next=NULL;

        current->next= newnode;// link previoust node to newnode
        current= newnode;// update current node as newnode

    }
}  
void print_linkedList(node* head)
{
     node* current=head;// for traversal
    while(current!=NULL)
    {
        printf("\n%d", current->data);
        
        current= current->next;
    }

}    
void insertNode_atFront(node** head, int data)// we have to make actual change to existing list, 
                                   // so pass by reference, also pass data of newnode
{
     // create a new node
    node* newnode= (node*)malloc(sizeof(node));
    newnode->data= data;
    newnode->next=NULL;

    newnode->next= *head;
    *head= newnode;// head from here on will be this newnode
}
void insertNode_atRear(node** head, int data)// passing reference is more Embedded style
{
    // create a new node
    node* newnode= (node*)malloc(sizeof(node));
    newnode->data= data;
    newnode->next=NULL;// last node

    // Get the pointer to last node 
    node* current= *head;
    while(current->next!=NULL)
    {
          current=current->next;
    }
    current->next=newnode;
}                             
void deleteNode_atFront(node** head)
{
    node* temp= *head;// use a temperory node to free memory

    *head = (*head)->next;// update head node as next node
                          // use brackets : -> has more precedence than *
    free(temp);           // free previous head
   
}
void deleteNode_atRear(node** head)
{
    // Get pointer to second last node
    node* current= *head;
    while(current->next->next!=NULL)
    {
        current=current->next;
    }
    // Current->next is the last node, frre it
   free(current->next);
   current->next=NULL;// set next node of second last node as NULL, making it the last node
   
}
void insertNode_atPosition(node** head, int pos, int data) // pass pointer to head ,
                                                           // position to insert newnode, data of newnode
{
    // Create a newnode to insert
    node* newnode= (node*)malloc(sizeof(node));
    newnode->data= data;
    newnode->next=NULL;

    // get pointer to (pos-1)th node
    node* current=*head;
    for(int i=1;i<pos-1;i++)
    {
        current=current->next;
    }
    // Now, current->next means the position node, we need newnode at that position
    // first, link newnode with position node
    // then,  link (pos-1) node with newnode
    newnode->next= current->next;
    current->next= newnode;

}
void deleteNode_atPosition(node** head, int pos)
{
    // Hanlde an Edge case : if pos=1 (head pointer) the loop won't run 
    //                     : means we would clear the second node (current->next)
    //                     : so, handle that case first
    if(pos==1)
    {
        node* temp= *head;   // temp pointer to free
        *head= (*head)->next;// update head as next node
        free(temp);          // either do this, or call our delete_atFront() 
        return;
    }

    // Like in insertion, get pointer to (pos-1)th node
    node* current= *head;
    for(int i=1;i<pos-1;i++)
    {
        current=current->next;
    }
    // link (pos-1)th node with (pos+1)node
    // then free the pos node
    node* temp= current->next;// create a temp pointer to free pos node
    current->next= current->next->next;
    free(temp);
    
}
void reverse_linkedList(node** head)
{
    // Logic : we flip the direction of pointers in each node
    // instead of pointing to next, each node point to previous node
    node* after=NULL;
    node* previous=NULL;
    node* current=*head;

    while(current!=NULL)// traverse through list
    {
        // Store next pointer for traversal
        after= current->next;

        // reverse direction of pointer
        current->next= previous;

        // update both previous and current for moving to next node
        previous=current;
        current=after;

    }
     // At the end of loop, current becomes NULL and previous becomes last node
     // list is reversed, so last node(previous) becomes new head 

     *head = previous;  // IMPORTANT: update head
}
static int list_Empty(node* head)
{
    // Call this function before every linked list operations
    // Handle the case when list is empty
    if(head==NULL){printf("\nLIST IS EMPTY"); return 1;}
    else return 0;
}
int print_listlength(node* head)
{
    if(list_Empty(head)){ return 0;}

    int len=0;
    node* current= head;
    // traverse through entire list
    while(current!=NULL)
    {
        current=current->next;
        len++;
    }
    printf("\nLIST LENGTH = %d", len);
    return len;

}
int main()
{
    // create a new node
    node* head= (node*)malloc(sizeof(node));
    head->data= 10;
    head->next=NULL;// no new node yet
    printf("\n%d", head->data);

    // Create next 6 nodes

    create_nodes(&head, 6);// pass by reference to make actual change to linked list

    //insertNode_atFront(&head,5);// 5 upfront

    //insertNode_atRear(&head,100);// 100 at last

    //deleteNode_atFront(&head);// remove 5

    //deleteNode_atRear(&head);// remove 100

    insertNode_atPosition(&head, 4, 45);// insert 45 at 4th position

    deleteNode_atPosition(&head, 5);// delete 5th node

    reverse_linkedList(&head)

    print_linkedList(head);

    print_listlength(head);

    return 0;
}