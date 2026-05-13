#include<stdio.h>
#include<limits.h>

void find_second_largest_smallest(int *a, int n)
{
    // Edge cases: empty array or single element array
    if(n==0 || n==1)
    {
        printf("\nInsufficient elements");
        return ;
    }
    // initialise variables with extreme values:
    int lar= INT_MIN, sec_lar= INT_MIN, sma= INT_MAX, sec_sma= INT_MAX;

    for(int i=0;i<n;i++)
    {
        // Find second largest:
        if(a[i]>lar)
        {
            sec_lar= lar;
            lar= a[i];
        }
        else if(a[i]>sec_lar && a[i]!=lar)
        {
            sec_lar= a[i];
        }
        // Find second smallest:
        if(a[i]<sma)
        {
            sec_sma= sma;
            sma= a[i];
        }
        else if(a[i]<sec_sma && a[i]!=sma)
        {
            sec_sma= a[i];
        }

    }
    // Edge case:
    if(sec_lar==INT_MIN || sec_sma==INT_MAX)
    {
        printf("\nAll elements are same");
        return ;
    }
    // Print second largest and second smallest
    printf("\nSecond largest : %d \nSecond smallest : %d",sec_lar,sec_sma);

}
// Define directions:
typedef enum{ RIGHT, LEFT}direction;

void shift_elements(int *a, int n, int k, direction dir)
{
    // Edge cases: empty array or single element array
    if(n==0 || n==1)
    {
        printf("\nInsufficient elements");
        return ;
    }
    int temp[n],new_index;
    // Traverse through array: find new index, store shifted elements
    for(int i=0;i<n;i++)
    {
        // If right shifting:
        if(dir==RIGHT)
        {
           new_index= (i + k) % n;
        }
        else
        {
            new_index= (i- k + n ) % n;
        }
        temp[new_index]=a[i];
    }
    // Copy elements back to original array:
    for(int i=0;i<n;i++)
    {
        a[i]= temp[i];
    }

}
int main()
{
    int arr[30],n,k;
    char d;
    direction dir;

    printf("Enter array size                    : ");
    scanf("%d",&n);
    printf("\nEnter array elements                : ");
    for(int i=0;i<n;i++)
    {
       scanf("%d",&arr[i]);
    }
    find_second_largest_smallest(arr, n);
   //printf("\nEnter number of positions to rotate : ");
    //scanf(" %d",&k);
    //printf("\nEnter the direction (R/L)           : ");
    //scanf(" %c",&d);
  //  if(d=='R') dir=RIGHT;
   // if(d=='L') dir=LEFT;

    //shift_elements(arr,n,k,dir);

   // printf("\nRotated array                       : ");
    //for(int i=0;i<n;i++)
    //{
      // printf("%d ",arr[i]);
    //}
}