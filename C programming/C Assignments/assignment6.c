#include<stdio.h>
                                              /* REMOVING CONSECUTIVE SPACES FROM A STRING */
void remove_multiple_spaces(char *str)
{
    // Edge case:
    if(*str=='\0')
    {
        printf("Empty string");
        return;
    }
    // Use two pointers i,j
    int i=0,j=0, space_found=0;
    while(str[i]!='\0')
    {
        // Write only when:
        // - character is not a space
        // - Extra consecutive spaces are skipped.
        if(str[i]!=' ')
        {
            str[j++]= str[i];
            space_found=0;
        } 
        else if(!space_found)
        {
            str[j++]= str[i];
            space_found=1;
        }
        i++;
    }
    // Null termination at end
    str[j]='\0';
}
int main()
{
    char string[40];
    printf("Enter the string : ");
    fgets(string, sizeof(string), stdin);

    remove_multiple_spaces(string);
    printf("\nResulting String : %s", string);

    return 0;
}