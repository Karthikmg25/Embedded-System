#include<stdio.h>

int remove_duplicates(int *arr, int n)
{
    // Edge cases: empty array or single element array
    if(n==0 || n==1)
    {
        printf("Insufficient elements");
        return n;
    }
    // Flag for detecting duplicates, index for writing unique elements:
    int unique, k=0;
    for(int i=0;i<n;i++)
    {
        // Assume element is unique:
        unique=1;
        // Check with unique elements:
        for(int j=0;j<k;j++)
        {
            // If element repeats,
            if(arr[i]==arr[j])
            {
                unique=0;
                break;
            }
        }
        // Write into the array if element is unique:
        if(unique)
        {
            arr[k++]= arr[i];
        }

    }
    // Return size of new array:
    return k;
}
int main()
{
    int arr[30],n;
    printf("Enter array size     : ");
    scanf("%d",&n);
    printf("\nEnter array elements : ");
    for(int i=0;i<n;i++)
    {
       scanf("%d",&arr[i]);
    }

    int new_size= remove_duplicates(arr,n);

    printf("\nUnique elements      : ");
    for(int i=0;i<new_size;i++)
    {
       printf("%d ",arr[i]);
    }
}