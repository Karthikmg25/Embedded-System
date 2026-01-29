#include<stdio.h>
#include "pointersort.h"
int element(int *p,int s,int e)// searching for a number
{
    int l=0;
    for(int i=0;i<s;i++)
    {
        if(*p==e)
         l++;
         p++;
    }
    return l;
}
int sum(int *p,int s)//finding the sum of elements
{
    int sum=0;
    for(int i=0;i<s;i++)
    {
        sum+=*p;
        p++;
    }
    return sum; 
}
float average(int *p,int s)//finding the average
{
    int sum=0;
    float a;
    for(int i=0;i<s;i++)
    {
        sum+=*p;
        p++;
    }
    a=(float)sum/s;
    return (a); 
}
int secondlargest(int *p,int s)
{
    int l=*p,sl=*p,*r=p;
    for(int i=0;i<s;i++)
    {
        if(l<*p)
        l=*p;
        p++;
    }
    
    
    return l;
}
void main()
{
    int a[20],n,e,sl=0,l;
    printf("enter the sise : ");
    scanf("%d",&n);
    printf("\nenter the elements : ");
    for(int i=0;i<n;i++)
    {
        scanf(" %d",&a[i]);
    }
    printf("\nenter the number : ");
    scanf(" %d",&e);
    if(element(a,n,e)!=0)
    printf("\nNUMBER IS PRESENT  ");
    else
    printf("\nNUMBER IS ABSENT ");
    printf("\n\nsum of the elements : %d ",sum(a,n));
    printf("\n\naverage of the elements : %.2f ",average(a,n));
    l=secondlargest(a,n);
    for(int i=0;i<n;i++)
    {
        if(a[i]!=l && sl<a[i])
        sl=a[i];
    }
    printf("\n\nsecond largest of the elements : %d \n",sl);
    printf("\nSORTED ARRAY : ");

    sort(a,n);                          //calling function from th file pointersort.h
    for(int i=0;i<n;i++)
    {
        printf(" %d",a[i]);
    }

      


}