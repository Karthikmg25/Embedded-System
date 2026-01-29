#include<stdio.h>
void bitbinary(int n)                          
{                                  //USING BIT MASKING TO FIND BINARY           
    int bit;
    for(int i=7;i>=0;i--)// starting from MSB(considering 8 bits)
    {
        bit=n & (1<<i);  // stores decimal value of n&(1<<i) into bit
        if(bit!=0)       // print 1 if bit is non zero else print 0
        printf(" 1");
        else
        printf(" 0");

    }
        
}
void main()
{
    int a,b,c;                                       
    printf("enter the two numbers between 0 & 255(8 bit numbers) \n");
    scanf("%d %d",&a,&b);
    if(a>255||b>255)  // 
    {
        printf("\nINVALID INPUTS");
        return;
    }

    printf("\nbinary of %d                  :",a);   bitbinary(a);
    printf("\nbinary of %d                  :",b);   bitbinary(b);
    printf("\nAND operation - %d & %d       :",a,b); bitbinary(a&b);
    printf("\nOR operation  - %d | %d       :",a,b);     bitbinary(a|b);
    printf("\nXOR operation - %d ^ %d       :",a,b);     bitbinary(a^b);  //  BASIC BITWISE OPERATIONS
    printf("\nNOT operation - ~%d           :",a);   bitbinary(~a) ;   
    printf("\nNOT operation - ~%d           :",b);   bitbinary(~b) ;      
    
    printf("\n\n enter another number(0-255) : ");
    scanf("%d",&c);
    printf("\nbinary of %d                       :",c); bitbinary(c);
    printf("\nleft shift by  1 - %d<<1           :");   bitbinary(c<<1) ; // left shift by 1
    
    printf("\nright shift by 1 - %d>>1           :");   bitbinary(c>>1) ; // right shift by 1
    
                                                                          // MULTIPLY AND DIVIDE USING SHIFT OPERATORS

    printf("\ndecimal value of %d<<1             : %d",c, c<<1);          // multiplication by 2 to the power of 1   
    printf("\ndecimal value of %d>>1             : %d",c, c>>1);          // division by 2 to the power of 1  
}