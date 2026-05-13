#include <stdio.h>
#include <string.h>
void string_reverse(char *str)
{
    int len= strlen(str),j;
    j= len-1;
    for(int i=0;i< len/2;i++)
    {
        // swap left and right characters of string
        char temp= str[i];
        str[i]= str[j];
        str[j]=temp;

        j--;
    }
}
void convert_to_base(int n, int base, char *str)
{
    // This function converts a decimal number to a given base(2-16)
    // Result is then stored in a string
    int rem,i=0;
    // Special case: if n is 0
    if(n == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    // generate digits in reverse order
    while(n!=0)
    {
        rem= n%base;
         // For bases > 10, represent 10–15 as A–F
        if(rem>=10)
        {
           str[i++]= (rem-10) + 'A';// store remainder as A-F character
        }
        else
        {
           str[i++]= rem + '0';// store remainder as a character   
        }
        n=n/base;
    }
    str[i]='\0';// add null termination

    // reverse the string for getting correct order
    string_reverse(str);

}
int main()
{
    int n, base;
    char str[33]; // Enough for 32-bit binary + null terminator
    printf("Enter the decimal number : ");
    scanf("%d",&n);
    printf("\nEnter the target base: ");
    scanf("%d",&base);
    
    convert_to_base(n, base, str);
    printf("\nConverted number in base %d: %s ",base,str);

}  