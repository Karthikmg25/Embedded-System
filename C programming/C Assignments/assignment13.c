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
    printf("\n\nResulting complex number : %d + i%d ",sum.real,sum.img);
   

}
