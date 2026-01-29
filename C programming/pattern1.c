#include<stdio.h>
#include<stdlib.h>
void pattern2(int n)
{  
   int g=1;
   while(g<=n)
   { 
      printf("\n");
    for(int i=0;i<g;i++)
    {
        printf(" %c",'*');
    }
    g++;
  }
}
void pattern1(int n)
{
    while(n>0)
  { 
       printf("\n");
    for(int i=1;i<=n;i++)
    {
        printf(" %c",'*');
    }
    n--;
   }
}
void main()
{
    int a;
    printf("enter the limit :");
    scanf("%d",&a);
    system("cls");
    printf("patterns\n");
    pattern1(a);
    printf("\n");
    pattern2(a);

}