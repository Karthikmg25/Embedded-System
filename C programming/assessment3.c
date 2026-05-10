#include<stdio.h>
#include<limits.h>
int secondlargest(int *p,int n)
{
    int l=INT_MIN,sl=INT_MIN;
    for(int i=0;i<n;i++)
    {
        
        if(p[i]>l)
        {
            sl=l;
            l=p[i];
        }
        else if( p[i]>sl && p[i]!=l)
        {
            sl= p[i];
        }
       
    }
      // If second largest was never updated
    if(sl == INT_MIN)
    { return -1;}
    return sl;
}
void main()
{
    int n,a[20];
    printf("how many elements          : ");
    scanf("%d",&n);
    printf("\nenter the elements       : ");
    for(int i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }
    printf("\nsecond largest element   : %d",secondlargest(a,n));

}
