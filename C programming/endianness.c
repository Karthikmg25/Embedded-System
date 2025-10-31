#include<stdio.h>
void main()
{
    unsigned int x= 0x12345678;  // hexa decimal number consisting 4 bytes
                                 // 78 - least significant byte
                                 // 12 - most significant byte
    unsigned char *p=(char *)&x; // cast address of x to a char pointer
                                 // so we can access the individual bytes of x 

    for(int i=0;i<4;i++)         // prints 4 characters ,each of size 1 byte
    {
        printf(" %02x",*(p+i));  // %x-for hexadecimal data, %2x-prints 2 characters 
                                 // %02x-pad with 0 if less than 2 digits (e.g., 0a)
    } 
    
    if(*p==0x78)                  // if LSB gets stored at first, system is little endian

     printf("\nSYSTEM IS LITTLE ENDIAN");
    else
       printf("\nSYSTEM IS BIG ENDIAN");



}
