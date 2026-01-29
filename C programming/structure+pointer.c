#include<stdio.h>
typedef struct
{
    int no,price;
    char name[20],author[20];
    
}book;

void main()
{
    book bk;// DECLARING A STRUCTURE VARIABLE

    book *b;// DECLARING A STRUCTURE POINTER

    b=&bk;  // STORE ADDRESS OF STRUCTURE VARIABLE IN POINTER

    printf(" enter the book number : ");
    scanf("%d",&b->no);// ACCESING STRUCTURE ELEMENT USING POINTER BY ARROW OPERATOR ->
                       // -> HAS HIGH PRECEDENCE THAN &
    getchar();         // always use getchar() while using scanf and fgets together- to clear newline from the inpput buffer                  
    printf(" enter the book name : ");
    fgets(b->name,20,stdin);
    printf(" enter the author name : ");
    fgets(b->author,20,stdin);
    printf(" enter the book price : ");
    scanf("%d",&b->price);
    printf("\n\n   BOOK DETAILS \n ");
    printf("\n book number : %d",b->no);
    printf("\n book name : ");
    fputs(b->name,stdout);
    printf(" author name : ");
    fputs(b->author,stdout);
    printf(" book price : %d",b->price);





                       
}