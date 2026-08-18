
#include "linked_list.h"

int main()
{
    node* head = List_CreateNodes(3);

    List_print(head);

    List_InsertAtPosition(&head, 10, 1);
    List_print(head);

    List_Reverse(&head);
    List_print(head);

}