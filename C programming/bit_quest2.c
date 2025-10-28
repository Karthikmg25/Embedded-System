#include<stdio.h>
void binary(int n)
{
    for(int i=31;i>=0;i--)
    {
        if((1<<i)&n)
         printf(" 1");
        else
         printf(" 0"); 
    }
}
void main()
{
    int num,s1=0,s0=0;
    printf("enter the number : ");
    scanf("%d",&num);
    for(int i=0;i<32;i++)
    {
        if((num>>i)&1)
        s1++;
        else
        s0++;
    }
    printf("\nbynary of number : ");binary(num);
    printf("\nnumber of 1's    : %d",s1);
    printf("\nnumber of 0's    : %d",s0);

}