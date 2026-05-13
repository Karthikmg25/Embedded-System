#include<stdio.h>

void print_valid_pairs(int *arr, int n, int target_sum)
{
    // Edge case:: Empty array  
    //           : Array with only one element
    if(n==0 || n==1)
    {
        printf("\nInsufficient elemenents");
        return;
    }
    
    int i,j,valid_pairs=0;

    // Find possible pairs:
    for( i=0;i<n;i++)
    {
        for( j=i+1;j<n;j++)
        {
            if(arr[i] + arr[j]== target_sum)
            {
                printf("\nValid pair : (%d , %d)", arr[i], arr[j]);
                // Valid pairs found:
                valid_pairs=1;
            }
        }
    }
    // edge case: No valid pairs found
    if(valid_pairs==0)
    {
        printf("\nNo valid pairs found");
    }

}
int main()
{
    int arr[40],n,target;
    printf("Enter the array size   : ");
    scanf("%d",&n);
    printf("\nEnter array elements : ");
    int i;
    for( i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }
    printf("Enter the target sum   : ");
    scanf("%d",&target);

    print_valid_pairs(arr, n, target);

    return 0;
}