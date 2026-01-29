#include<stdio.h>
void main()
{
    int a[5]={1,3,5,4,15},*p;
    
    p=a;
        printf(" \n\n%d",*p);
        printf(" \n\n%d",++*p);// incrments the value of *p
    printf(" \n\n%d",*++p);//first incriments value of p and prints *(p+1)
    
    printf(" \n\n%d",*p++);  //prints the value of *p then increments the value of p
     printf(" \n\n%d",*p);
         printf(" \n\n%d",*--p);// decrememtes value of p and stores value of *(p-1)

}