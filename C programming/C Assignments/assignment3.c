#include<stdio.h>
#include<string.h>
void string_reverse(char *str)
{
    int len= strlen(str);
    int i=0,j=len-1;
    //  if number is negative, reverse numerical part only
    if(str[0]=='-')
    {
        i=1;// skip -ve sign
    }
    
    while( i<j)
    {
      
        // swap characters from left and right to reverse 
        char temp= str[i];
        str[i]=str[j];
        str[j]=temp;
        i++;
        j--;
    }
}

void NumberToString(int n, char *str)
{
    int i=0;
    // handle case when n=0
    if(n == 0)
    {
    str[0] = '0';
    str[1] = '\0';
    return;
    }
    // handle case when n is negative
    if(n < 0)
    {
      str[i++] = '-';// store -ve sign upfront
      n = -n;// remove sign of number
    }
    
    // store each digits of number into array in reverse order
    // reverse the string at the end
    while(n!=0)
    {
        int digit= n%10;
        str[i++]= digit+'0';
        n=n/10;
    }
    str[i]='\0';// null termination

    string_reverse(str);// reverse for correct order
}
int main()
{
   int n;
   char string[20];
   printf("Enter the number : ");
   scanf("%d",&n);
   NumberToString(n,string);
   printf("\nnumber string : %s", string);
   return 0;
}