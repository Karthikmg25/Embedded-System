#include<stdio.h>
int letter_position(char ch)
{
    
    if(ch>='A'&&ch<='Z')// if uppercase letter,ASCII vale between 65 and 90
    return ch-64;       // 64='A'-1
    else
    if(ch>='a'&& ch<='z')// if uppercase letter,ASCII value between 97 and 122 
    return ch-96;        // 96= 'a'-1
    else
    return 0;    // return 0 if not an alphabet                   
}
void main()
{
    char a;
    printf("enter the alphabet : ");scanf(" %c",&a);
    printf("\nletter position   : %d",letter_position(a));
}