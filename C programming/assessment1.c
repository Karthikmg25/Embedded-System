#include<stdio.h>
void binary(int x)
{
    int bit;
    for(int i=31;i>=0;i--)
    {
        bit=(x>>i)&1;
        printf(" %d",bit);
    }
}
void bitmanip(int x)
{
    x=x|(1<<2);
    printf("\n\nvaalue after setting the 3rd bit : %d",x);
    printf("\nbinary after setting the 3rd bit : ");binary(x);
    
    x=x&~(1<<5);
    printf("\n\nvalue after resetting the 6th bit : %d",x);
    printf("\nbinary after resetting the 6th bit : ");binary(x);
    x=x^(1<<0);
    printf("\n\nvalue after toggling the 1st bit : %d",x);
    printf("\nbinary after toggling the 1st bit : ");binary(x);

}
void main()
{
     int n;
     printf("enter the number : ");
     scanf("%d",&n);
     printf("binary of the number : ");binary(n);
     bitmanip(n);

}
