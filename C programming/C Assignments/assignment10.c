#include<stdio.h>

int merge_sorted_arrays(int* a, int n, int* b, int m, int* c)
{
    // Edge case:
    if(n==0 || m==0)
    {
        printf("One or both arrays are empty");
        return 0;
    }
    // Use poinetrs for traversal and writing:
    int i=0,j=0,k=0;
    // Loop till one array is processed:
    while(i<n && j<m)
    {
        // Find smaller elemeent, write to third array and update pointers:
        if(a[i]<b[j])
        {
            c[k++]=a[i];
            i++;
        }
        else
        {
            c[k++]=b[j];
            j++;
        }

    }
    // Copy remaining elements
    while (i<n)
    {
        c[k++]=a[i];
        i++;
    }
    while (j<m)
    {
        c[k++]=b[j];
        j++;
    }
    // return new size
    return k;

}
int main()
{
    int a[30],b[30],n,m,c[60];

    printf("Enter size of first array  : ");
    scanf("%d",&n);
    printf("\nEnter first sorted array   : ");
    for(int i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }
    printf("\nEnter size of second array : ");
    scanf("%d",&m);
    printf("\nEnter second sorted array  : ");
    for(int i=0;i<m;i++)
    {
        scanf("%d",&b[i]);
    }

    int new_size= merge_sorted_arrays(a, n, b, m, c);

    printf("\nArray after merging  : ");
    for(int i=0;i<new_size;i++)
    {
        printf("%d ",c[i]);
    }

}