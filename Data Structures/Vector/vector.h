#ifndef VECTOR_H
#define VEctor_H

#include <stdio.h>
#include <stdlib.h>

#define VECTOR_CAPACITY   5

//define vector
typedef struct 
{
    int *data;
    size_t size;
    size_t capacity;

}vector_t;
/*
size increases as we perform pushback,
size indicates index of last element + 1
*/
// vector operations:
void vector_init(vector_t *v);
void vector_pushback(vector_t *v, int value);
void vector_print(vector_t *v);
void vector_popback(vector_t *v);
void vector_capacity(vector_t *v);
void vector_size(vector_t *v);
int vector_get(vector_t *v, int index);
/*
fn: updates value of an existing element
*/
void vector_setValue(vector_t *v, size_t index, int value);
/*
fn: checks if vector is empty or not
*/
int vector_empty(vector_t *v);
/*
fn: resets the size of vector as 0 (doesn't free memory)
*/
void vector_clear(vector_t *v);
/*
fn: destroys the vector
*/
void vector_free(vector_t *v);
/*
fn: insert element at a position
*/
void vector_insert(vector_t *v, size_t index, int value);
/*
fn: erase the element at the index
*/
void vector_erase(vector_t *v, size_t index);
/*
fn: search an element and return index if present
return: -1 if no element found
*/
size_t vector_search(vector_t *v, int element);


#endif