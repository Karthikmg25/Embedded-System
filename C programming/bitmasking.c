#include<stdio.h>
#include "bitbinary.h"
void main()
{
    int n,b;
    char ch;
    printf("enter the number(0-255)\n");
    scanf("%d",&n);
    if(n>255)
    {
        printf("\nINVALID INPUT");
        return;
    }
    printf("\nbinary of the number : "); bitbinary(n);
    printf("\n\n              bit masking  ");
    printf("\n\nwhich operation do you want to perform -set,reset,toggle ?(s/r/t) ");
    scanf(" %c",&ch);
    switch(ch)
    {
        case 's' : printf("\nenter the index of the bit you want to set ? ");
                   scanf("%d",&b);

                   n=n |(1<<b);                                     // perform OR operation with the mask

                   printf("\nnumber after setting : "); bitbinary(n);
                   break;
        case 'r' : printf("\nenter the index of the bit you want to reset ? ");
                   scanf("%d",&b);

                   n=n &~(1<<b);                                   // perform AND operation with the compliment of mask

                   printf("\nnumber after resetting : "); bitbinary(n);
                   break;
        case 't' : printf("\nenter the index of the bit you want to toggle ? ");

                   scanf("%d",&b);                                // perform XOR operation with the mask

                   n=n ^(1<<b);
                   printf("\nnumber after toggling : "); bitbinary(n);
                   break;
        default  : printf("\n WRONG OPTION");
                   break;         
    }
}