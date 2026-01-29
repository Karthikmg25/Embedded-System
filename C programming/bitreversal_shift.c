#include<stdio.h>
void shiftreverse(int *n)
{
    int bit, rev=0;
    for(int i=0;i<32;i++)
    {
       bit= ((*n)>>i) & 1; // takes the LSB when i=0
       rev= (rev<<1) | bit; // adds bit as LSB of rev  
                            // and on the next iteration we shift the number left to add the next bit
                            // at the end of loop , LSB of n will be the MSB of rev - reverse
    }
    *n=rev; // assign reveresd value to number 
}
void binary(int n)
{
    for(int i=31;i>=0;i--)
    {
        if((1<<i)&n)
         printf(" 1");
        else
         printf(" 0"); 
          
    
    }
}
void main()
{
    int a;
    printf("enter the number to be reversed : ");
    scanf("%d",&a);
    printf("\nnnumber in binary           : ");  binary(a);
    shiftreverse(&a); // call by reference/address
    printf("\nreversed number in binary   : ");  binary(a);
    printf("\nreversed number in decimal  : %d",a);
    

}