#include<stdio.h>

void rearrange_array(int *arr, int n)
{
    // Edge case:: Empty array  
    //           : Array with only one element
    if(n==0 || n==1)
    {
        printf("\nInsufficient elemenents");
        return;
    }
    
    int i,j, temp[n],k=0;

    // store negative elements to temp[]:
    for( i=0;i<n;i++)
    {
        if(arr[i]<0)
        {
            temp[k++] = arr[i];
        }
    }
    // store non-negative elements to temp[]:
    for( i=0;i<n;i++)
    {
        if(arr[i]>=0)
        {
            temp[k++] = arr[i];
        }
    }
    // Copy back to original array:
    for( i=0;i<n;i++)
    {
             arr[i] =temp[i];
        
    }

}
int main()
{
    int arr[40],n;
    printf("Enter the array size   : ");
    scanf("%d",&n);
    printf("\nEnter array elements : ");
    int i;
    for( i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }

    rearrange_array(arr, n);

    printf("\nRearranged array    : ");

    for( i=0;i<n;i++)
    {
        printf("%d ",arr[i]);
    }

    return 0;
}