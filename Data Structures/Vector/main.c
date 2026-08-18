
#include "vector.h"

int main()
{
    vector_t v;
    //initialize vector: allocate space
    vector_init(&v);

    vector_pushback(&v, 10);
    vector_pushback(&v, 20);
    vector_pushback(&v, 30);
    vector_pushback(&v, 40);
    vector_pushback(&v, 50);
    //push data more than capacity
    vector_pushback(&v, 60);
    vector_insert(&v, 2, 25);
    vector_print(&v);
    
   printf("\n%d", vector_search(&v, 25));
    
    return 0;
}