
#include<stdio.h>
int sum(int a,int b)
{
   return (a+b);
}


int dif(int a,int b)
{
   return (a-b);

}
int product(int a,int b)
{
   return (a*b);

}
float div(float a,float b)
{
   if(b== 0)
      printf("ERROR");
   return (a/b);

}


int main()
{
    int a,b;
    int ch;
    char h;
    do{
     printf("\n\n\n");              
     printf("              CALCULATOR\n");
     printf("             ____________\n\n");
     
     printf("enter two numbers \n ");
     scanf("%d%d",&a,  &b);
     printf("\nselect:\n1.addition \n2.substraction\n3.multiplication\n4.division\n");
     scanf("%d",&ch);
     switch (ch)
     {
      case 1 : printf("\nsum of two numbers :%d",sum(a,b));
                break; 
      case 2 : printf("\ndiffrence of two numbers :%d",dif(a,b));
                 break; 
      case 3 : printf("\nproduct of two numbers :%d",product(a,b));
                 break; 
      case 4 : printf("\ndivision of two numbers :%.3f",div(a,b));
                 break; 
    default  :   printf("\nwrong operator") ;
     }
      printf("\n\ndo you want to continue?(y/n)");
          scanf(" %c",&h);  
      } while(h=='y');      
              
               
     return 0;
}     

