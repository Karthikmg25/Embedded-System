#include<stdio.h>
#include<stdlib.h>
int main()
{
    int *p,n,m;
    printf("how many numbers to enter ? ");
    scanf("%d",&n);

    p=(int *)malloc(n*sizeof(int));             // MALLOC--allocating memory for n elements, returns a pointer towards the memory 

                                   // (int*)- type casting it to an integer type pointer--optional in C,mandatory in C++
    if(p==NULL)                    // returns a null pointer if memory is not suffecient for allocation
    {                              // a null pointer points to nothing
        printf("\nMEMORY NOT ALLOCATED");
        return 1;                  // to exit from the function, means abnormal termination of the program 
    }
    printf("\n enter the elements \n");
    for(int i=0;i<n;i++)
    {
        scanf(" %d",p+i);
    }
    printf("\n printing the elements \n");
    for(int i=0;i<n;i++)
    {
        printf(" %d",*(p+i));
    }

    p=(int *)calloc(n,sizeof(int));              // CALLLOC--allocating memory for n elements at the same address and INITIALISE DATA AS ZERO
    
    printf("\n\n printing the elements after using calloc function\n");
    for(int i=0;i<n;i++)           // all elements become zero
    {
        printf(" %d",*(p+i));
    }
    printf("\n\n how many elements do you want to add ? \n");
    scanf("%d",&m);

    p=(int *)realloc(p,(n+m)*sizeof(int));       // REALLOC-- resizing the memory to add m more elements, new size becomes (n+m)*sizeof(int)

    if(p==NULL)                   
    {                              
        printf("\nMEMORY NOT ALLOCATED");
        return 1;                  
    }
       printf("\n enter the new elements \n");
    for(int i=n;i<n+m;i++)
    {
        scanf(" %d",p+i);
    }
  
   
    printf("\n printing the elements after resizing memory\n");
    for(int i=0;i<n+m;i++)           // all elements 
    {
        printf(" %d",*(p+i));
    }

    free(p);


}