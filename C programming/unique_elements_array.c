#include<stdio.h>
void unique(int a[],int n)
{
    int flag;
    for(int i=0;i<n;i++)
    {
        flag=0;
        for(int j=0;j<n;j++)
        {
            if(j!=i && a[i]==a[j])// compare each element with every other element to check uniqueness
            {
            flag=1;// if element repeats ,sef flag to 1, not unique 
            break;
            }

        }
        if(flag==0)
        printf(" %d",a[i]);

    }
}
int main()
{
    int a[25],n;
    printf("enter the limit : ");scanf("%d",&n);
    printf("enter the elements : ");
    for(int i=0;i<n;i++)
    {
        scanf(" %d",&a[i]);
    }
    printf("unique elements in the array : ");
    unique(a,n);
    return 0;
}