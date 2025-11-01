#include<stdio.h>
void main()
{
    int n,l=1,space;
    printf("enter the limit : ");
    scanf("%d",&n);
    space=n;
    while(l<=n)
    { printf("\n");
        for(int i=space;i>0;i--)
         printf(" ");
    for(int i=0;i<l;i++)
     printf(" *");
     l++;
     space--;
    }
}
