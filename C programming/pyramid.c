#include<stdio.h>
void pyramid(int n)
{
    for(int i=0;i<n;i++)
    {
        printf("\n");
    for(int j=0;j<n;j++)
    {
        if((i+j)>=n-1)
        printf(" ^");
        else
        printf(" ");
    }
   }
}
void main()
{
    int a;
    printf("enter the limit ");
    scanf("%d",&a);
    printf("\n");
    pyramid(a);
}
