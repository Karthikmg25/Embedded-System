#include<stdio.h>
int decimal(int b)
{
    int dec=0, lsb;
    for(int i=0;b!=0;i++)
    {
        lsb= b%10;           // taking the LSB of number
        dec= dec+ (lsb<<i);  // multiplying with power of 2 and taking sum
        b=b/10;              // divide by 10 to get the next bit
    }
    return dec;
}
void main()
{
    int n;
    printf("enter binary number         : ");
    scanf("%d",&n);
    printf("\ndecimal of the number     : %d",decimal(n));

}