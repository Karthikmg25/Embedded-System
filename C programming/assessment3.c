#include<stdio.h>
int secondlargest(int *p,int n)
{
    int l=*p,sl=*p;
    for(int i=0;i<n;i++)
    {
        if(l<*(p+i))
        l=*(p+i);
    }
    for(int i=0;i<n;i++)
    {
        
        if(sl<*(p+i) && *(p+i)!=l)
        sl=*(p+i);
    }
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
