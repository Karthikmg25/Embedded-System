#include<stdio.h>
void case_toggle(char str[])
{   
    int i;
    for(i=0;str[i]!='\0';i++)
    {
        if(str[i]>='A' && str[i]<='Z')      // if letter is UPPERCASE,
        str[i]+=32;                         //   convert to LOWERCASE

        else        
        if(str[i]>='a' && str[i]<='z')      //         if LOWERCASE,
        str[i]-=32;                         // convert to UPPERCASE
    }
    
}
void main()
{
    char a[30];
    printf("enter the string : "); fgets(a,30,stdin);
    case_toggle(a);
    printf("\nstring afetr case toggling : ");printf("%s",a);
   
}