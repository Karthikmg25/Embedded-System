#include<stdio.h>

void bitbinary(int n)
{
    int bit;
    for(int i=31;i>=0;i--)
    {
        bit=n & (1<<i);
        if(bit!=0)
        printf(" 1");
        else
        printf(" 0");

    }
        
}
void main()
{
    int n;
    printf("enter the number(limit-32 bit)\n");
    scanf("%d",&n);
    printf("\nbinary of the number\n");
    bitbinary(n);
}