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
int toggle(int n,int b)
{
    n=n^(1<<b);
    return n;
}
void main()
{
    int num,bit,t,tog,b;
    printf("enter the number                       : ");
    scanf("%d",&num);
    b=num;
    printf("\nbinary of the number                 : "); binary(num);

    printf("\nwhich bit do you want to set ?       : ");
    scanf("%d",&bit);
    num=num| (1<<bit);
    printf("\nbinary of the number after setting   : "); binary(num);
    printf("\ndecimal of the number after setting  : %d ",num);
    printf("\n\nwhich bit do you want to toggle ?    : ");
    scanf(" %d",&t);
    tog=toggle(b,t);
  
    printf("\nbinary of the toggled number         : "); binary(tog);
    printf("\n toggled number                      : %d",tog);
    
}