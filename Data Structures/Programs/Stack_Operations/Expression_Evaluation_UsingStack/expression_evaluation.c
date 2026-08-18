
#include "stack.h"

int main()
{
    stack_t stack= {.top= -1};

    // post-fix expression for evaluation
    char expression[]= "12*3+";// infix version -> (1 * 2) + 3

    int i=0;
    while(expression[i]!= '\0')
    {
        unsigned char ch = expression[i];
        // push to stack if character is an operand:
        // -- convert char to digit
        if(ch >= '0'&& ch<= '9' )
        {
            stack_push(&stack, ch-'0');
        }
        // If character is Operator:
        // -- pop last 2 operands from stack and perform the operation
        // -- the result will be then pushed back to stack
        else 
        {
            int op1= stack_pop(&stack);
            int op2= stack_pop(&stack);
            // Variable for storing intermediate results
            int int_result;

            switch (ch)
            {
                case '+': int_result = op1 + op2;
                      stack_push(&stack, int_result);
                      break;
                case '-': int_result = op1 - op2;
                      stack_push(&stack, int_result);
                      break;
                case '*': int_result = op1 * op2;
                      stack_push(&stack, int_result);
                      break;
                case '/': int_result = op1 / op2;
                      stack_push(&stack, int_result);
                      break;         
            
                default: printf("\nInvalid Operator in Expression");
                break;
            }
        }
        i++;
        
    }
    // Final result will be stored in stack:
    printf("\n Evaluation result: %d", stack_pop(&stack));

}