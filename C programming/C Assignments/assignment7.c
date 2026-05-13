#include<stdio.h>
                                              /*BITWISE OPERATIONS*/
void toggle_bits(int *num, int n, int pos)
{
    // Edge case,1: No bits to toggle
    if(n==0)
    {
        printf("No bits toggled");
        return;
    }
    // Edge case,2: Invalid position
    if(pos<0 || pos>=(sizeof(int)*8))
    {
        printf("Invalid position");
        return ;
    }
    // Edge case,3: Invalid number of bits
    if(n<0 || n> ((sizeof(int) * 8) -pos))
    {
        printf("Invalid number of bits");
        return ;
    }
    // Create a mask of n 1's
    int mask= ((1<<n)-1)<<pos;
    // Toggle n bits
    *num^= mask;
}
int extract_bits(int num, int n, int pos)
{
    // Edge case,1: No bits to be extracted
    if(n==0)
    {
        printf("No bits to be extracted");
        return -1;
    }
    // Edge case,2: Invalid position
    if(pos<0 || pos>=(sizeof(int)*8))
    {
        printf("Invalid position");
        return -1;
    }
    // Edge case,3: Invalid number of bits
    if(n<0 || n> ((sizeof(int) * 8) -pos))
    {
        printf("Invalid number of bits");
        return -1;
    }
    // Create a mask of n 1's
    int mask= ((1<<n)-1)<<pos;
    // Extract n bits
    int bits= num & mask;
    // Shift back to LSB
    bits=bits>>pos;

    return bits;
}
void replace_bits(int *dest, int src, int n, int pos)
{
    // Edge case,1: No bits to be replaced
    if(n==0)
    {
        printf("No bits to be replaced");
        return ;
    }
    // Edge case,2: Invalid position
    if(pos<0 || pos>=(sizeof(int)*8))
    {
        printf("Invalid position");
        return ;
    }
    // Edge case,3: Invalid number of bits
    if(n<0 || n> ((sizeof(int) * 8) -pos))
    {
        printf("Invalid number of bits");
        return ;
    }
    // Create a mask of n 1's
    int mask= ((1<<n)-1)<<pos;
    // Copy n bits from source
    int bits= src & mask;
    // Clear n bits of destination
    *dest=*dest & ~mask;
    // Insert source bits into destination
    *dest =*dest| bits;
    
}
void swap_bits(int *a, int *b, int n, int pos)
{
    // Edge case,1: No bits to swap
    if(n==0)
    {
        printf("No bits swapped");
        return;
    }
    // Edge case,2: Invalid position
    if(pos<0 || pos>=(sizeof(int)*8))
    {
        printf("Invalid position");
        return ;
    }
    // Edge case,3: Invalid number of bits
    if(n<0 || n> ((sizeof(int) * 8) -pos))
    {
        printf("Invalid number of bits");
        return ;
    }
    // Loop through n bits from given position
    for(int i=pos;i<(pos+n);i++)
    {
        int bit_a = (*a>>i)&1;
        int bit_b = (*b>>i)&1;
        // If bits aren't same, toggle both bits
        if(bit_a!=bit_b)
        {
            *a^=(1<<i);
            *b^=(1<<i);
        }
    }
   
}
int main()
{
    int a,b,n,position;
    printf("Enter first number                    : ");
    scanf("%d",&a);
    printf("\nEnter second number                   : ");
    scanf("%d",&b);
    printf("\nEnter number of bits to be extracted  : ");
    scanf("%d",&n);
    printf("\nEnter position                        : ");
    scanf("%d",&position);
    
    swap_bits(&a,&b,n,position);

    printf("\nResult after swap                     : %d  %d", a, b);

    return 0;
}