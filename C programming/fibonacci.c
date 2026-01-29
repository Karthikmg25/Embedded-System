#include<stdio.h>
void main()
{
    int i=0,j=1,t=0,s=0,n,k;
    printf("how many numbers do you want?\n");
    scanf("%d",&n);
    printf("FIBONACCI\n%d\t%d",i,j);
    k=2;//2 numbers are already printed,we've to print n-2 numbers in loop,so initialise k=2
    while(k<n)
    {
        t=i+j;
          printf("\t%d",t);
          s=s+t;
          i=j;
          j=t;
          k++;
    }
    printf("\nsum=%d",s+1);

}