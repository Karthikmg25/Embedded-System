#include<stdio.h>
void swap(int *a,int *b) // use call by reference method to reflect the change in values to the main function 
{
    *a=*a^*b;
    *b=*a^*b;
    *a=*a^*b;
}
void main()
{
    int a,b;
    printf("\nenter the first number\n");
    scanf("%d",&a);
    printf("\nenter the second number\n");
    scanf("%d",&b);
    swap(&a,&b);  // pass the address of the numbers to function
    printf("\nnumbers after swaping : %d %d ",a,b);
    
}