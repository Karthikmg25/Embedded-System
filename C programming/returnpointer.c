#include <stdio.h>
int* arraysort(int n)
{
    static int a[25];
    printf("\nenter the elements\n ");
    for(int i=0;i<n;i++)
    {
        scanf("\n %d",&a[i]);
    
    }
    for(int i=0;i<n;i++)
    {
    for(int j=i+1;j<n;j++)
    {
        if(a[i]>a[j])
        {
            a[i]=a[i]^a[j];
            a[j]=a[i]^a[j];
            a[i]=a[i]^a[j];
            
        }
    
    }
    }
    return a;
        
    
    
}

int main() 
{
    int *p,n;
    printf("enter the size of array : ");
    scanf("%d",&n);
    p=arraysort(n);
    printf("sorted array \n");
    for(int i=0;i<n;i++)
    {
        printf("\n %d",*(p+i));
    
    }
    
  

}

