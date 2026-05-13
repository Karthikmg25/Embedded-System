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
    int n, freq[10]={0};
    
    printf("Enter the number ");
     scanf("%d", &n);
    
    count_frequency(n, freq);
   
    for(int i=0;i<10;i++)
    {
        if(freq[i]>0)
        {
            printf("\nFrequency of digit %d = %d",i, freq[i]);
        }
        
    }  
    return 0;
}