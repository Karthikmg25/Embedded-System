#include<stdio.h>
#include<string.h>

int pangram(char *str)
{
    // Edge case
    if(str[0]=='\0' )
    {
        printf("Not a Pangram");
        return 0;
    }
    // Use a frequency array to mark presence of alphabets
    int freq[26]={0},i=0, index;

    // Mark alphabets
    while(str[i]!='\0')
    {
        // Map character to array-index based on case, mark presence

        if(str[i]>='A' && str[i]<='Z')// Uppercase
        {
            index= str[i]- 'A';
            freq[index]=1;
        }
        else if(str[i]>='a' && str[i]<='z')// Lowercase
        {
            index= str[i]-'a';
            freq[index]=1;
        }
        i++; 
       
    }
    // Check frequency array
    for(int i=0;i<26;i++)
    {
        // If any alphabet is missing 
        if(freq[i]==0)
        {
            return 0;// not a pangram
           
        }    
    }

    return 1;// is pangram

}
int main()
{
    char string[30];
    printf("Enter your string : ");
    fgets(string, sizeof(string),stdin);// use fgets to include space

    if(pangram(string))
    {
        printf("\nString is Pangram");
    }
    else
    {
        printf("\nString is not a Pangram");
    }
    
}