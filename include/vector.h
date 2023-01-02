#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 5
#define GROWTH_FACTOR    2

typedef struct
{
  int* array;
  int size;
  int capacity;
} vector;

/*
 * Initializes a vector of integers
 * capacity = INITIAL_CAPACITY
 * size = 0
 *
 * A vector is a data structure that allows:
 *    O(1) amortized insert.
 *    O(1) search.
 *    Delete is not implemented since it will be not needed.
 *
 * @returns new vector or null
 * */
vector* vector_init( );

/*
 * Push back means it will add to the end of the array.
 * If there is not enough space to insert an item,
 * the vector will grow according to the GROWTH_FACTOR.
 *
 * @returns EXIT_SUCCESS or EXIT_FAILURE
 * */
int vector_push_back(vector* vector, int item);

/*
 * Removes element at index
 *
 * @returns the element
 * */
int vector_remove_at(vector* vector, int index);

void vector_print(vector* vector);

/*
 * Frees the memory used.
 * */
void vector_free(vector* vector);

bool vector_is_empty(vector* vector);

int vector_size(vector* vector);

#endif
