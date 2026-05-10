# C Assignment 

## 1. Write a C program that takes an integer from the user and determines whether the number is even or odd.

### Logic:
* Check if the number is divisible by 2
* If remainder is zero, number is even, else odd

### Code:
```
#include<stdio.h>

void check_even_odd(int n)
{
    if(n%2==0)
    {
        printf("Number is Even");
    }
    else
    {
        printf("Number is Odd");
    }
}
int main()
{
    int n;
    printf("Enter the number ");
    scanf("%d",&n);

    check_even_odd(n);

    return 0;
}
```
## 2.Write a C program that asks the user to enter a number and determines whether that number is prime or not.
### Logic:
* Check if the given number have any factors between 2 and square root of number.
* If no factors, number is Prime.
* Edge case : If number is less than or equal to one, it is neither Prime nor Composite.
### Code:
```
#include<stdio.h>

void IsPrime(int n)
{
    if(n<=1)// Edge case
    {
        printf("Number is Not Prime");
        return;
    }
    
    // Loop from 2 to square root of n
    // if n is not prime, one of its factors will be <= sqrt(n)
    for(int i=2;i*i<=n;i++)
    {
        if(n%i==0)
        {
            printf("Number is Not Prime");
           return;
        }

    }
    
        printf("Number is Prime");
   
}
int main()
{
    int n;
    printf("Enter the number ");
    scanf("%d",&n);

    IsPrime(n);

    return 0;
}
```
## 3.Write a C program that takes a positive integer from the user and calculates its factorial.
### Logic:
* Factorial of a number is equal to 1 * 2 * 3 *...* n.
* Find product of numbers from 1 to n.
* Edge case: Factorial is defined only for non-negative integers.
### Code:
```
#include<stdio.h>

long long factorial(int n)
{
    if(n<0)// invalid input
    {
        return -1;
    }
    if(n==0)
    {
        return 1;
    }
    int fact=1;
    // To get factorial, find product of numbers from 1 to n, factorial= 1*2*3*...*n
    for(int i=1;i<=n;i++)
    {
        fact= fact*i;
    }
   
   return fact;
}
int main()
{
    int n;
    long long fact;
    printf("Enter the number ");
    scanf("%d",&n);

    fact= factorial(n);
    if(fact==-1)
    {
        printf("\nInvalid Input");
    }
    else
    {
        printf("\nFactorial of the number : %d", factorial(n));
    }
    
    return 0;
}
```
## 4.Write a C program to generate the Fibonacci series up to a given number of terms.
### Logic:
* Sequence starts with value 0,1.
* Each next number is sum of last two numbers.
* Edge case: When n<=0, nothing to print.
### Code:
```
#include<stdio.h>

void print_Fibonacci(int n)
{
    if(n<=0)
    {
        printf("Invalid Input");
        return ;
    }
  int a=0, b=1, next;
  // each iterations updates a with b and b with a+b
  for(int i=0;i<n;i++)// print n terms
  {
    printf("%d ",a);

    next=a+b;
    a=b;
    b=next;

  }
}
int main()
{
    int n;
    
    printf("Enter the number ");
    scanf("%d",&n);

    printf("\nFibonacci Series\n");
    print_Fibonacci(n);
    
    return 0;
}
```
## 5.Write a C program that uses a function to check whether a given number is a Perfect Number.
### Logic:
* Perfect number means the sum of all proper factors(excluding the number itself) is equal to the number.
* Find sum of all proper divisors of number except that number and compare with the number.
* Edge case : Perfect number is defined only for non-negative integers.
            : 1 is not a perfect number.
### Code:
```
#include<stdio.h>

int IsPerfect(int n)
{
    if(n<=0 || n==1)// Edge case
    {
        return -1;
    }
  int sum=0;
  // find sum of all proper divisors of number except that number
  for(int i=1;i<=n/2;i++)
  {
    if(n%i==0)// if i is a factor, add it to sum
    {
        sum+=i;
    }
  }
   if(sum==n)
    {return 1;}// perfect number
    else
    {return 0;}// not a perfect number
}
int main()
{
    int n, result;
    
    printf("Enter the number ");

    scanf("%d",&n);

    result= IsPerfect(n);

    if(result==-1){

        printf("\nInvalid Input");
    }
    else if(result== 1){

        printf("\nNumber is Perfect");
    }
    else{
         printf("\nNumber is not Perfect");
    }
    
   
    
    return 0;
}
```
## 6.Write a C program that uses a recursive function to find the GCD of two numbers using the Euclidean algorithm.
### Logic:
* According to Euclidean algorithm, GCD(a,b) = GCD(b, a%b).
* In recursion, pass first argument as `b` and second argument as `a%b`.
* Return b when `a%b==0`(the GCD)
* Edge case : If b == 0, return a
        
### Code:
```
#include<stdio.h>

int gcd(int a, int b)
{
    // Base condition:
    // If second number becomes 0, first number is the GCD
    if(b==0)
    {
        return a;
    }
    // Euclidean Algorithm:
    // GCD(a, b) = GCD(b, a % b)
    return gcd(b, a%b);
}
int main()
{
    int n1,n2, result;
    
    printf("Enter the numbers ");
    scanf("%d %d",&n1,&n2);

    if(n1<=0 || n2<=0)
    {
        printf("\nInvalid Input");
        return 0;
    }
    printf("\nGCD of Numbers : %d", gcd(n1,n2));

    
    return 0;
}
```
## 7.Write a C program that uses a function to count the frequency of each digit (0–9) in a given integer.
### Logic:
* Use an array to store frequency of each digits(0-9), `freq[10]`
* Loop through each digits of the number to find frequency.
* Print if digit appear atleast once in the number.
* Edge case : If the number is 0, treat it as a valid input and count frequency of digit 0 as 1.
        
### Code:
```
#include<stdio.h>

void count_frequency(int n, int *freq)
{
    // If number is 0, frequency of digit 0 is 1
    if(n==0)
    {
        freq[0]++;
        return ;
    }
  // traverse through each digits of number and update the frequency array
  // freq[i] stores frequency of digit i (0–9)

  while(n!=0)
  {
    int digit= n%10;
    freq[digit]++;
    n=n/10;
  }
}
int main()
{
    int n, freq[10]={0};// initialise array as 0 to avoid garbage values
    
    printf("Enter the number ");
    scanf("%d", &n);
    
    count_frequency(n, freq);
   
    for(int i=0;i<10;i++)
    {
        if(freq[i]>0)// print only if digit appears atleast once
        {
            printf("\nFrequency of digit %d = %d",i, freq[i]);
        }
        
    }  
    return 0;
}
```
## 8.Write a C program that uses functions to convert a decimal number to any base between 2 and 16.
### Logic:
* Read decimal number and target base (2 to 16).
* Repeatedly divide the number by the base.
* Store the remainder each time (these form the converted number in reverse order).
* Map remainders 10–15 to characters A–F.
* Reverse the string for final output
* Edge case : If number is 0, store string as '0'  
            : if number is negative,convert using absolute value and handle sign separately.
        
### Code:
```
#include <stdio.h>
#include <string.h>

void string_reverse(char *str)
{
    
    int len= strlen(str),j,i=0;
    j= len-1;
    // case: if string is of a negative number
    if(str[0] == '-') i = 1;  // skip sign during reverse
    while(i<j)
    {
        // swap left and right characters of string
        char temp= str[i];
        str[i]= str[j];
        str[j]=temp;
        i++;
        j--;
    }
}
void convert_to_base(int n, int base, char *str)
{
    // This function converts a decimal number to a given base(2-16)
    // Result is then stored in a string
    int rem,i=0;
    //  case: if n is 0
    if(n == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    // Case: if n is negative
    if(n < 0)
    {
    str[i++] = '-';//store '-' in string
    n = -n;// convert number to positive
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

    if(base<2 || base>16)// chack if base is valid
    {
        printf("\nInvalid base");
        return 0;
    }
    
    convert_to_base(n, base, str);
    printf("\nConverted number in base %d: %s ",base,str);

}  
```
## 9.Write a C program that uses functions to evaluate a simple arithmetic expression given two numbers and an operator.
### Logic:
* Read two numbers and an operator (+, -, *, /, %).
* Use a function that takes both numbers and operator as arguments.
* Inside the function, use switch-case to perform the required operation.
* Return the result to main() and print it.
* Edge case : Division by zero → should be handled (invalid message).   
            : Invalid operator input → return error / invalid  operation.
        
### Code:
```
#include<stdio.h>
float Add(float a, float b)
{
   return a+b;
}
float Subtract(float a, float b)
{
   return a-b;
}
float Multiply(float a, float b)
{
   return a*b;
}
float Divide(float a, float b)
{
    if(b==0)
    {
        printf("Error : division by zero");
        return 0;
    }
   return a/b;
}
float Operation(float a, float b, char operator)
{
    float result;
    switch (operator)
    {
        case '+' : result=Add(a,b);
                   return result;
        case '-' : result=Subtract(a,b);
                   return result;          
        case '*' : result=Multiply(a,b);
                   return result;
        case '/' : result=Divide(a,b);
                   return result;      
        default  : printf("Invalid Operator");
                   return 0;                          
    }

}
int main()
{
    float a,b;
    char op;
    printf("Enter the two numbers : ");
    scanf("%f %f",&a,&b);
    printf("Enter the operator : ");
    scanf(" %c",&op);
    printf("\nResult : %.2f", Operation(a,b,op));

    return 0;
}
```
## 10.Write a C program that takes a string of digits from the user and converts it into its equivalent integer value.
### Logic:
* Read each characters in the string through a loop.
* Convert each character to number using `str[i] - '0'`
* Build integer using n = n * 10 + digit.
* Edge case : If first character is -, store sign separately and apply at end.
            
        
### Code:
```
#include<stdio.h>

int StringToNumber(char *str)
{
    int n=0, i=0, sign=1;
    // Case: string is of a negative number
    if(str[0]=='-') { i=1; sign= -1;}// skip negative sign('-') & store sign

    while(str[i]!='\0')
    {

        int digit= str[i]-'0';// convert each character to digit
        n=n*10 + digit;// use digits to buld the number
        i++;
    }
    

    return n*sign;
}
int main()
{
    char string[20];
   printf("Enter the string of digits");
   scanf("%s", string);
   printf("\nInteger number : %d", StringToNumber(string));
    return 0;
}
```
## 11.Write a C program that takes an integer from the user and converts it into a string of characters.
### Logic:
* Read each digits of the integer.
* Convert each digit to character using `digit + '0'` to buld the string (in reverse order).
* Reverse string at the end for correct order.
* Edge case : If number is 0, store string as '0'  
            : if number is negative,convert using absolute value and handle sign separately.
            
        
### Code:
```
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
```
## 12.Write a C program that takes a string from the user and checks whether it is a palindrome.
### Logic:
* A string is considered a palindrome when It reads the same forward and backward.
* Compare characters from both ends of the string.
* If characters are same-> palindrom.
* Edge case : Empty strings and single-character strings are considered palindrom. 
            
            
        
### Code:
```
#include<stdio.h>
#include<string.h>
int palindrom_check(char *str)
{
    // Edge case
    if(*str=='\0' || str[1]=='\0' )
     return 1;// string is palindrom

    // compare characters from right and left ends
    int palindrom=1;// use a flag to check

    for(int i=0,j=strlen(str)-1; i<j; i++,j--)
    {
       if(str[i]!=str[j])
       {
        palindrom=0;// not a palindrom
        break;
       }
    }
    if(palindrom)
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
```
## 13.Write a C program that takes a string from the user and reverses its characters using an iterative approach.
### Logic:
* Loop through the string to read characters from both ends.
* Swap characters from both ends.
* Edge case: Nothing to reverse if string is empty or have single character.

### Code:
```
#include<stdio.h>
#include<string.h>

void string_reverse(char *str)
{
    // Edge case
    if(str[0]=='\0' || str[1]=='\0')
    {
        printf("Nothing to reverse");
        return;
    }
    // pointers for character swapping
    int i=0,j= strlen(str)-1;
    // Swap characters from both ends
    while(i<j)
    {
        char temp=str[i];
        str[i]=str[j];
        str[j]= temp;

        i++; j--;
    }

}
int main()
{
    char string[30];
    printf("Enter your string : ");
    scanf("%s",string);
                                            // Reverse the string
    string_reverse(string);
                                            // Print the resulting string
    printf("\nReversed string : %s",string);
}
```
## 14.Write a C program that takes a string input from the user and determines whether it is a pangram or not.
### Logic:
* A string is considered pangram if it contains all alphabets from a-z(A-Z) at least once.
* Use a frequency array( `freq[26]`)and map characters to index to mark presence of each alphabets.
* Check if all alphabets are present using the frequency array.
* Edge case: Handle Empty strings  
           : Handle case sensitivity (uppercase/lowercase)

### Code:
```
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
```
## 15.Write a C program that takes a string from the user and replaces every sequence of multiple consecutive spaces with a single space.
### Logic:
* Use a two pointer approach, one for traversing through string and one for writing into same string.
* Detect multiple consecutive spaces using a flag `space_found`.
* Write characters into the same string while skipping extra consecutive spaces.
* Edge case: Empty string ("")  
           : String with only spaces (" ")  
           : String with no extra spaces  
### Code:
```
#include<stdio.h>

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
```     
## 16.Write a C program that takes a string from the user and converts all uppercase letters into their corresponding lowercase letters.      
### Logic:
* Traverse the string character by character.
* Use ASCII difference between uppercase and lowercase letters for conversion.
* Lowercase = Uppercase + `('a'-'A')`
* Perform conversion when an uppercase letter is found.
* Replace the character with its lowercase equivalent.
* Edge case: Empty string ("")  
           : String containing no uppercase letters- remains unchanged.
            
### Code:
```
#include<stdio.h>
void to_lowercase(char *str)
{
    // Edge case:
    if(*str=='\0')
    {
        printf("Empty string");
        return;
    }
    int i=0;
    while(str[i]!='\0')
    {
        // If Uppercase, convert to lowercase:
        if(str[i]>='A' && str[i]<='Z')
        {
            str[i]= str[i]+('a'-'A');
        }
        i++;
    }
}
int main()
{
    char string[40];
    printf("Enter your string : ");
    fgets(string, sizeof(string), stdin);

    to_lowercase(string);
    printf("\nResulting String : %s", string);

    return 0;
}
```
## 17.Write a C program that takes a number and toggles a specific number of bits (n) starting from a given position.
### Logic:
* Create a mask containing n consecutive 1's.
* Shift the mask to given position.
* Use XOR (`^`) operation with the number to toggle the selected bits.
* Toggling operation :  
    result = number ^ mask
* Edge case: `n`=0(no bits toggled).
           : Invalid bit position(negative / outside  bit-width of integer).   
           : `n` exceeding available bits from given position.  
           
            
### Code:
```
#include<stdio.h>
void toggle_bits(int *num, int n, int pos)
{
    // Edge case,1: No bits to toggle
    if(n==0)
    {
        printf("No bits toggled");
        return;
    }
    // Edge case,2: Invalid position
    if(pos<0 || pos>=(sizeof(int)*8))
    {
        printf("Invalid position");
        return ;
    }
    // Edge case,3: Invalid number of bits
    if(n<0 || n> ((sizeof(int) * 8) -pos))
    {
        printf("Invalid number of bits");
        return ;
    }
    // Create a mask of n 1's
    int mask= ((1<<n)-1)<<pos;
    // Toggle n bits
    *num^= mask;
}
int main()
{
    int num,n,position;
    printf("Enter your number                  : ");
    scanf("%d",&num);
    printf("\nEnter number of bits to be toggled : ");
    scanf("%d",&n);
    printf("\nEnter position                     : ");
    scanf("%d",&position);
    // Pass number by reference:
    toggle_bits(&num,n,position);
    
    printf("\nResulting number                   : %d ",num);

    return 0;
}
```
## 18.Write a C program that takes an integer from the user and extracts (n) bits starting from a specific bit position.
### Logic:
* Create a mask containing n consecutive 1's.
* Shift the mask to given position.
* Extract bits using AND operarion : `bits=num&mask`.
* Shift result back to LSB: `bits >> pos`.
* Print result.  
* Edge case: `n=0`(no bits extracted).  
           : Invalid bit position(negative / outside  bit-width of integer).   
           : `n` exceeding available bits from given position.             
### Code:
```
#include<stdio.h>

int extract_bits(int num, int n, int pos)
{
    // Edge case,1: No bits to be extracted
    if(n==0)
    {
        printf("No bits to be extracted");
        return -1;
    }
    // Edge case,2: Invalid position
    if(pos<0 || pos>=(sizeof(int)*8))
    {
        printf("Invalid position");
        return -1;
    }
    // Edge case,3: Invalid number of bits
    if(n<0 || n> ((sizeof(int) * 8) -pos))
    {
        printf("Invalid number of bits");
        return -1;
    }
    // Create a mask of n 1's
    int mask= ((1<<n)-1)<<pos;
    // Extract n bits
    int bits= num & mask;
    // Shift back to LSB
    bits=bits>>pos;

    return bits;
}
int main()
{
    int num,n,position;
    printf("Enter your number                      : ");
    scanf("%d",&num);
    printf("\nEnter number of bits to be extracted : ");
    scanf("%d",&n);
    printf("\nEnter position                       : ");
    scanf("%d",&position);
    
    printf("\nResulting number                     : %d ",extract_bits(num,n,position));

    return 0;
}
```
## 19.Write a C program that takes a number and replaces n of its bits, starting from a given position, with another set of n bits from a second number.
### Logic:
* Create a mask containing n consecutive 1's.
* Shift the mask to given position.
* Copy n bits from the source         : `bits= src & mask`
* Clear n bits in destination         : `dest&=~mask`
* Insert source bits into destination : `dest|=bits`
* Edge case: `n=0` (no bits replaced).  
           : Invalid bit position(negative / outside  bit-width of integer).   
           : `n` exceeding available bits from given position.             
### Code:
```
#include<stdio.h>

void replace_bits(int *dest, int src, int n, int pos)
{
    // Edge case,1: No bits to be replaced
    if(n==0)
    {
        printf("No bits to be replaced");
        return ;
    }
    // Edge case,2: Invalid position
    if(pos<0 || pos>=(sizeof(int)*8))
    {
        printf("Invalid position");
        return ;
    }
    // Edge case,3: Invalid number of bits
    if(n<0 || n> ((sizeof(int) * 8) -pos))
    {
        printf("Invalid number of bits");
        return ;
    }
    // Create a mask of n 1's
    int mask= ((1<<n)-1)<<pos;
    // Copy n bits from source
    int bits= src & mask;
    // Clear n bits of destination
    *dest=*dest & ~mask;
    // Insert source bits into destination
    *dest =*dest| bits;
    
}
int main()
{
    int dest,src,n,position;
    printf("Enter your destination number         : ");
    scanf("%d",&dest);
    printf("\nEnter your source number              : ");
    scanf("%d",&src);
    printf("\nEnter number of bits to be extracted  : ");
    scanf("%d",&n);
    printf("\nEnter position                        : ");
    scanf("%d",&position);
    
    replace_bits(&dest,src,n,position);

    printf("\nResulting number                     : %d", dest);

    return 0;
}

```
## 20.Write a C program that takes two integers and swaps n bits starting from a given position in both numbers
### Logic:
* Loop through `n` bits of both integers starting from the given position.
* Extract corresponding bits from both numbers.
* If bits are diffrent, toggle both bits to swap them.
* Edge case: `n=0` (no bits replaced).  
           : Invalid bit position(negative / outside  bit-width of integer).   
           : `n` exceeding available bits from given position.             
### Code:
```
#include <stdio.h>


void swap_bits(int *a, int *b, int n, int pos)
{
    // Edge case,1: No bits to swap
    if(n==0)
    {
        printf("No bits swapped");
        return;
    }
    // Edge case,2: Invalid position
    if(pos<0 || pos>=(sizeof(int)*8))
    {
        printf("Invalid position");
        return ;
    }
    // Edge case,3: Invalid number of bits
    if(n<0 || n> ((sizeof(int) * 8) -pos))
    {
        printf("Invalid number of bits");
        return ;
    }
    // Loop through n bits from given position
    for(int i=pos;i<(pos+n);i++)
    {
        int bit_a = (*a>>i)&1;
        int bit_b = (*b>>i)&1;
        // If bits aren't same, toggle both bits
        if(bit_a!=bit_b)
        {
            *a^=(1<<i);
            *b^=(1<<i);
        }
    }
   
}
int main()
{
    int a,b,n,position;
    printf("Enter first number                    : ");
    scanf("%d",&a);
    printf("\nEnter second number                   : ");
    scanf("%d",&b);
    printf("\nEnter number of bits to be extracted  : ");
    scanf("%d",&n);
    printf("\nEnter position                        : ");
    scanf("%d",&position);
    
    swap_bits(&a,&b,n,position);

    printf("\nResult after swap                     : %d  %d", a, b);

    return 0;
}
```
## 21.Write a C program to remove duplicate elements from a given array.
### Logic:
* Use nested loops to check whether each element is duplicated. 
* Maintain a separate index to store unique elements in the same array.
* Write the element only if it is found to be unique.
* Edge case: Empty array.  
           : Array with only one element.  
           : Array containing all duplicate elements.  
### Code:
```
#include<stdio.h>

int remove_duplicates(int *arr, int n)
{
    // Edge cases: empty array or single element array
    if(n==0 || n==1)
    {
        printf("Insufficient elements");
        return n;
    }
    // Flag for detecting duplicates, index for writing unique elements:
    int unique, k=0;
    for(int i=0;i<n;i++)
    {
        // Assume element is unique:
        unique=1;
        // Check with unique elements:
        for(int j=0;j<k;j++)
        {
            // If element repeats,
            if(arr[i]==arr[j])
            {
                unique=0;
                break;
            }
        }
        // Write into the array if element is unique:
        if(unique)
        {
            arr[k++]= arr[i];
        }

    }
    // Return size of new array:
    return k;
}
int main()
{
    int arr[30],n;
    printf("Enter array size     : ");
    scanf("%d",&n);
    printf("\nEnter array elements : ");
    for(int i=0;i<n;i++)
    {
       scanf("%d",&arr[i]);
    }

    int new_size= remove_duplicates(arr,n);

    printf("\nUnique elements      : ");
    for(int i=0;i<new_size;i++)
    {
       printf("%d ",arr[i]);
    }
}
```    
### Output:
```
Enter array size     : 6

Enter array elements : 12 34 24 12 34 27

Unique elements      : 12 34 24 27 
```  
## 22.Write a C program that uses an array to find the second largest and second smallest element in a list of numbers.
### Logic:
* Traverse the array to find the largest and second largest elements.
* When a new largest element is found, update second largest with previous largest value.
* Update the second largest if any element is larger than second largest and smaller than largest. 
* Use a similar approach to find the smallest and second smallest elements.
* Edge case: Empty array.  
           : Array with only one element.  
           : Array containing all same elements.  
### Code:     
```
#include<stdio.h>
#include<limits.h>

void find_second_largest_smallest(int *a, int n)
{
    // Edge cases: empty array or single element array
    if(n==0 || n==1)
    {
        printf("\nInsufficient elements");
        return ;
    }
    // initialise variables with extreme values:
    int lar= INT_MIN, sec_lar= INT_MIN, sma= INT_MAX, sec_sma= INT_MAX;

    for(int i=0;i<n;i++)
    {
        // Find second largest:
        if(a[i]>lar)
        {
            sec_lar= lar;
            lar= a[i];
        }
        else if(a[i]>sec_lar && a[i]!=lar)
        {
            sec_lar= a[i];
        }
        // Find second smallest:
        if(a[i]<sma)
        {
            sec_sma= sma;
            sma= a[i];
        }
        else if(a[i]<sec_sma && a[i]!=sma)
        {
            sec_sma= a[i];
        }

    }
    // Edge case: same elements
    if(sec_lar==INT_MIN || sec_sma==INT_MAX)
    {
        printf("\nAll elements are same");
        return ;
    }
    // Print second largest and second smallest
    printf("\nSecond largest : %d \nSecond smallest : %d",sec_lar,sec_sma);

}
int main()
{
    int arr[30],n;
    printf("Enter array size     : ");
    scanf("%d",&n);
    printf("\nEnter array elements : ");
    for(int i=0;i<n;i++)
    {
       scanf("%d",&arr[i]);
    }

    find_second_largest_smallest(arr,n);
}
```
### Output:
```
Enter array size                    : 6

Enter array elements                : 12 27 48 10 54 7

Second largest  : 48 
Second smallest : 10
```
## 23.Write a C program that rotates the elements of an array by n positions in a specified direction.
### Logic:
* Use a temporary array (temp) to store rotated elements.
* Traverse the original array and compute the new index for each element.
* If right rotation: `new_index = (i + n) % array_size`.
* If left rotation : `new_index = (i - n + array_size) % array_size`.
* Store each element in `temp[n]` array at new index. 
* Copy temp array back into the original array.
* Edge case: Empty array.  
           : Array with single element (rotation has no effect).  
           : `n` greater than array size (handled using modulo).  
             
### Code:
```
#include<stdio.h>

// Define directions:
typedef enum{ RIGHT, LEFT}direction;

void shift_elements(int *a, int n, int k, direction dir)
{
    // Edge cases: empty array or single element array
    if(n==0 || n==1)
    {
        printf("\nInsufficient elements");
        return ;
    }
    int temp[n],new_index;
    // Traverse through array: find new index, store shifted elements
    for(int i=0;i<n;i++)
    {
        // If right shifting:
        if(dir==RIGHT)
        {
           new_index= (i + k) % n;
        }
        else
        {
            new_index= (i- k + n ) % n;
        }
        temp[new_index]=a[i];
    }
    // Copy elements back to original array:
    for(int i=0;i<n;i++)
    {
        a[i]= temp[i];
    }

}
int main()
{
    int arr[30],n,k;
    char d;
    direction dir;

    printf("Enter array size                    : ");
    scanf("%d",&n);
    printf("\nEnter array elements                : ");
    for(int i=0;i<n;i++)
    {
       scanf("%d",&arr[i]);
    }
    printf("\nEnter number of positions to rotate : ");
    scanf(" %d",&k);
    printf("\nEnter the direction (R/L)           : ");
    scanf(" %c",&d);
    if(d=='R') dir=RIGHT;
    if(d=='L') dir=LEFT;

    shift_elements(arr,n,k,dir);

    printf("\nRotated array                       : ");
    for(int i=0;i<n;i++)
    {
       printf("%d ",arr[i]);
    }
}
```
### Output:
```
Enter array size                    : 6

Enter array elements                : 1 2 3 4 5 6

Enter number of positions to rotate : 2

Enter the direction (R/L)           : L

Rotated array                       : 3 4 5 6 1 2 
```
## 24.Write a C program that merges two sorted arrays into a single sorted array without using any sorting function.
### Logic:
* Use three pointers: i and j to traverse the two sorted arrays, and k for the merged array.
* Both arrays are sorted, means smaller elements are at the front.
* Compare elements at a[i] and b[j].(i and j start from 0)
* Copy the smaller element into the third array and move the corresponding pointer.
* Increment k after every insertion.
* After one array is fully traversed, copy remaining elements of the other array (always larger).
* Edge case: One or both arrays are empty 
           
             
### Code:
```
#include<stdio.h>

int merge_sorted_arrays(int* a, int n, int* b, int m, int* c)
{
    // Edge case:
    if(n==0 || m==0)
    {
        printf("One or both arrays are empty");
        return 0;
    }

    // Use poinetrs for traversal and writing:
    int i=0,j=0,k=0;

    // Loop till one array is processed:
    while(i<n && j<m)
    {
        // Find smaller elemeent, write to third array and update pointers:
        if(a[i]<b[j])
        {
            c[k++]=a[i];
            i++;
        }
        else
        {
            c[k++]=b[j];
            j++;
        }

    }

    // Copy remaining elements : Always larger than stored elements
    while (i<n)
    {
        c[k++]=a[i];
        i++;
    }
    while (j<m)
    {
        c[k++]=b[j];
        j++;
    }

    // return new size
    return k;

}
int main()
{
    int a[30],b[30],n,m,c[60];

    printf("Enter size of first array  : ");
    scanf("%d",&n);
    printf("\nEnter first sorted array   : ");
    for(int i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }
    printf("\nEnter size of second array : ");
    scanf("%d",&m);
    printf("\nEnter second sorted array  : ");
    for(int i=0;i<m;i++)
    {
        scanf("%d",&b[i]);
    }

    int new_size= merge_sorted_arrays(a, n, b, m, c);

    printf("\nArray after merging  : ");
    for(int i=0;i<new_size;i++)
    {
        printf("%d ",c[i]);
    }

}
```
### Output:
```
Enter size of first array  : 4

Enter first sorted array   : 12 37 39 58

Enter size of second array : 5

Enter second sorted array  : 7 19 28 35 46

Array after merging        : 7 12 19 28 35 37 39 46 58 
```
## 25. Write a C program that finds all pairs of elements in an array whose sum equals a given target value.
### Logic:
* Use nested loops to examine all possible pairs in the array.
* For each element, compare it with the remaining elements of the array.
* Check whether the sum of the pair equals the target value.
* Print the pair if a valid sum is found.
* Print a message if no valid pairs exist.
* Edge cases: Empty array  
            : Array with only one element  
            : No valid pairs found
### Code:
```
#include<stdio.h>

void print_valid_pairs(int *arr, int n, int target_sum)
{
    // Edge case:: Empty array  
    //           : Array with only one element
    if(n==0 || n==1)
    {
        printf("\nInsufficient elemenents");
        return;
    }
    
    int i,j,valid_pairs=0;

    // Find possible pairs:
    for( i=0;i<n;i++)
    {
        for( j=i+1;j<n;j++)
        {
            if(arr[i] + arr[j]== target_sum)
            {
                printf("\nValid pair : (%d , %d)", arr[i], arr[j]);
                // Valid pairs found:
                valid_pairs=1;
            }
        }
    }
    // edge case: No valid pairs found
    if(valid_pairs==0)
    {
        printf("\nNo valid pairs found");
    }

}
int main()
{
    int arr[40],n,target;
    printf("Enter the array size   : ");
    scanf("%d",&n);
    printf("\nEnter array elements : ");
    int i;
    for( i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }
    printf("Enter the target sum   : ");
    scanf("%d",&target);

    print_valid_pairs(arr, n, target);

    return 0;
}
```
### Output:
```
Enter the array size   : 6

Enter array elements   : 24 12 18 25 8 6

Enter the target sum   : 30

Valid pair : (24 , 6)
Valid pair : (12 , 18)

```
## 26.Write a C program that rearranges an array such that all negative numbers appear before all positive numbers while preserving the relative order of both groups.
### Logic:
* Use a temperory array to storerearranged elements.
* Traverse the array and first copy all negative elements into `temp[]` while preserving their order.
* Traverse the array again and copy all non-negative elements into temp[] while preserving their order.
* Copy elements back into original array from `temp[]`.
* Edge case: : Empty array.    
             : Array with only one element.
### Code:
```
#include<stdio.h>

void rearrange_array(int *arr, int n)
{
    // Edge case:: Empty array  
    //           : Array with only one element
    if(n==0 || n==1)
    {
        printf("\nInsufficient elemenents");
        return;
    }
    
    int i,j, temp[n],k=0;

    // store negative elements to temp[]:
    for( i=0;i<n;i++)
    {
        if(arr[i]<0)
        {
            temp[k++] = arr[i];
        }
    }
    // store non-negative elements to temp[]:
    for( i=0;i<n;i++)
    {
        if(arr[i]>=0)
        {
            temp[k++] = arr[i];
        }
    }
    // Copy back to original array:
    for( i=0;i<n;i++)
    {
             arr[i] =temp[i];
        
    }

}
int main()
{
    int arr[40],n;
    printf("Enter the array size   : ");
    scanf("%d",&n);
    printf("\nEnter array elements   : ");
    int i;
    for( i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }

    rearrange_array(arr, n);

    printf("\nRearranged array       : ");

    for( i=0;i<n;i++)
    {
        printf("%d ",arr[i]);
    }

    return 0;
}
```
### Output:
```
Enter the array size   : 6

Enter array elements   : 12 -37 29 -68 39 -10

Rearranged array       : -37 -68 -10 12 29 39 

```
## 27. Define a structure Complex with real and imaginary parts. Write a C program to add two complex numbers and display the result.
### Logic:
* Define a structure `Complex` containing `real` and `imaginary` members.
* Create a function that accepts two complex numbers as arguments.
* Add corresponding real and imaginary parts separately.
* Return the resulting complex number (returning another structure).
* Edge case:  Complex numbers with zero real or imaginary parts.  
           :  Negative real or imaginary values  
### Code:
```
#include<stdio.h>

// define structure:
typedef struct
{
   int real;
   int img;
}complex;
// Create function:
complex add_complex(complex a, complex b)
{
    complex sum;

    sum.real = a.real + b.real;
    sum.img  = a.img  + b.img ;
    
    return sum;
}
int main()
{
    complex a,b,sum;
    // Enter input:
    printf("Enter first complex number ");
    printf("\nEnter real part      : "); scanf("%d",&a.real);
    printf("\nEnter imaginary part : "); scanf("%d",&a.img);
    printf("\n\nEnter Second complex number ");
    printf("\nEnter real part      : "); scanf("%d",&b.real);
    printf("\nEnter imaginary part : "); scanf("%d",&b.img);

    sum= add_complex(a,b);
    
    // Display output:
    printf("\n\nResulting complex number : %d + i %d ",sum.real,sum.img);
   

}

```
### Output:
```
Enter first complex number 

Enter real part      : 24

Enter imaginary part : 10


Enter Second complex number 

Enter real part      : 8

Enter imaginary part : 28


Resulting complex number : 32 + i38 

```           
 