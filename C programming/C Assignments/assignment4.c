#include<stdio.h>
#include<string.h>
int palindrom_check(char *str)
{

    // compare characters from right and left ends
    int flag=0;// use a flag to check

    for(int i=0,j=strlen(str)-1; i<j; i++,j--)
    {
       if(str[i]!=str[j])
       {
        flag=1;
        break;
       }
    }
    if(flag==0)
    return 1;// string is palindrom
    else
    return 0;// not a palindrom

}
int main()
{
    char string[40];
    printf("enter the string : ");
    scanf("%s",string);
    if(palindrom_check(string))
    {
        printf("string is palindrom");
    }
    else
    {
         printf("string is not palindrom");
    }
}