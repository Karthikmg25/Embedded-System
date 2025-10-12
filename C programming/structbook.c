#include<stdio.h>
#include<stdlib.h>
struct book
{
    char name[20],author[20];
    int price;

};
struct book b[3];
void largest(struct book b[])
{ 
    if((b[0].price>b[1].price)&&(b[0].price>b[2].price))
        printf("\nexpensive book is %s",b[0].name);
      
    
    else if((b[0].price<b[1].price)&&(b[1].price>b[2].price))
        printf("\nexpensive book is %s",b[1].name);
    
    else
        printf("\nexpensive book is %s",b[2].name);
    
}
void smallest(struct book b[])
{ 
    if((b[0].price<b[1].price)&&(b[0].price<b[2].price))
        printf("\ninexpensive book is %s",b[0].name);
      
    
    else if((b[0].price>b[1].price)&&(b[1].price<b[2].price))
        printf("\ninexpensive book is %s",b[1].name);
    
    else
      printf("\ninexpensive book is %s",b[2].name);
    
}



void main()

{ 
    
    
   
    for(int i=0;i<3;i++)
    {
        printf("\nenter details of book %d",i+1);
        printf("\n\nenter name        :");
        scanf(" %s",b[i].name);
        printf("enter author name  :");
        scanf(" %s",b[i].author);
        printf("enter price        :");
        scanf("%d",&b[i].price);
        system("cls");

    }   
    for(int i=0;i<3;i++)
    {
        printf("\n\ndetails of book %d",i+1);
        printf("\n-------------------");
        printf("\nname       :%s",b[i].name);
        printf("\nauthor name:%s",b[i].author);
        printf("\nprice      :%d",b[i].price);


    }  
    printf("\n"); 
    largest(b);
    smallest(b); 

}
