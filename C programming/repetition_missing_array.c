#include<stdio.h>
int repeating(int a[],int n)
{
    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            if(a[i]==a[j])
            return a[i];

        }
    }

}
int main()
{
    int a[20],n,sum,array_sum=0,missing;
    printf("enter the limit : ");scanf("%d",&n);
    printf("\nenter the elements : ");// from 1 to n
    for(int i=0;i<n;i++)
    {
    scanf(" %d",&a[i]);
    array_sum+=a[i];
    }
     printf("\nrepeating element : %d",repeating(a,n));
     sum=n*(n+1)/2;
     missing=repeating(a,n) + (sum-array_sum);
     printf("\nmissing element   : %d",missing);
     return 0;
}