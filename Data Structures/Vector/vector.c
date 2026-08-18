#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
/*
size increases as we perform pushback,
size indicates index of last element + 1
*/
// vector operations:
void vector_init(vector_t *v)
{
    v->data = NULL;
    v->size = 0;
    v->capacity = VECTOR_CAPACITY;

    //allocate the initial space for data
    v->data = (int*)malloc(v->capacity  * (sizeof(int)));
    if(v->data == NULL){
        printf("\nAllocation failed");
        return;
    }
}
void vector_pushback(vector_t *v, int value)
{
    //if enough space, store data on memory
    if(v->size < v->capacity)
    {
        v->data[v->size] = value;
        //update size
        v->size++;
    }
    else
    {
        //debugging
        printf("\nNew allocation");
        //allocate new space, 
        int *temp = (int*)realloc(v->data, 2*(v->capacity)*sizeof(int));
        if(temp == NULL){
        printf("\nAllocation failed");
        return;
        }
        v->data = temp;

        //insert value
        v->data[v->size] = value;

        //update size and double the capacity
        v->size++;
        v->capacity *= 2;
    }

}
void vector_print(vector_t *v)
{
    printf("\n");
    for(size_t i=0;i< v->size;i++)
    {
        printf("%d ", v->data[i]);
    }
}
void vector_popback(vector_t *v)
{
    //memory still exist, but element no longer belongs to vector.
    if(v->size>0)
    {
        v->size--;
    }
}
void vector_capacity(vector_t *v)
{
    printf("\nCurrent capacity: %d", v->capacity);
}

void vector_size(vector_t *v)
{
    printf("\nCurrent size: %d", v->size);
}
int vector_get(vector_t *v, int index)
{
    return v->data[index];
}
/*
fn: updates value of an existing element
*/
void vector_setValue(vector_t *v, size_t index, int value)
{
    v->data[index] = value;
}
/*
fn: checks if vector is empty or not
*/
int vector_empty(vector_t *v)
{
    return v->size == 0? 1:0;
}
/*
fn: resets the size of vector as 0 (doesn't free memory)
*/
void vector_clear(vector_t *v)
{
    v->size =0;
}
/*
fn: destroys the vector
*/
void vector_free(vector_t *v)
{
    free(v->data);

    v->data = NULL;
    v->capacity =0;
    v->size = 0;
}
/*
fn: insert element at a position
*/
void vector_insert(vector_t *v, size_t index, int value)
{
    //validate the index
    if(index > v->size)
    {
        printf("\nInvalid index");
        return;
    }
    if(v->size+1 > v->capacity)
    {
        //more space needed
        int* temp = (int*)realloc(v->data, 2*(v->capacity)*sizeof(int));
        if(temp == NULL)
        {
            printf("\nAllocation failed");
            return;
        }
        v->data = temp;
        v->capacity *= 2;
    }
    //shift elements from index to right by one place
    //start from last element
    for(size_t i = v->size; i>index;i--)
    {
        v->data[i] = v->data[i-1];
    }
    v->data[index] = value;
    v->size++;
}
/*
fn: erase the element at the index
*/
void vector_erase(vector_t *v, size_t index)
{
    //shift elements from right to left till index
    for(size_t i=index;i<v->size;i++)
    {
        v->data[i] = v->data[i+1];
    }
    //reduce size by one
    v->size--;

}
/*
fn: search an element and return index if present
return: -1 if no element found
*/
size_t vector_search(vector_t *v, int element)
{
    for(size_t i=0; i<v->size ;i++)
    {
        if(v->data[i] == element)
        {
            return i;
        }
    }
    return -1;//no element found
}
