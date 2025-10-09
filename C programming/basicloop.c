#include<stdio.h>
int main()
{
    int i,n;
    printf("enter a number \n");
    scanf("%d",&n);
    printf("numbers upto %d\n\n", n);
    for(i=1;i<=n;i++)
    {
    printf("%d\t", i);
    }
     return 0;
}