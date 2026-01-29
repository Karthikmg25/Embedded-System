#include<stdio.h>
int odd_occuring(int *a,int n)
{
    int xor=0;
    for(int i=0;i<n;i++)
    {
        xor^=*(a+i); // xor of all elements in the array
    }
    return xor;
}
void main()
{
    int a[30],lim;
    printf("enter the limit : ");scanf("%d",&lim);
    printf("\nenter the elements : ");
    for(int i=0;i<lim;i++)
    scanf(" %d",a+i);
    printf("\nodd occuring element in the array : %d",odd_occuring(a,lim));
}