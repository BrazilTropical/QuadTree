#include "vector.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Initialise an empty vector */
vector* vector_init( )
{
  vector* vector;
  int* vector_pointer;

  vector         = malloc(sizeof(vector));
  vector_pointer = malloc(sizeof(int) * INITIAL_CAPACITY);

  if ( vector_pointer == NULL )
  {
    fprintf(stderr, "Unable to allocate memory, exiting.\n");
    free(vector_pointer);
    return NULL;
  }

  vector->array    = vector_pointer;
  vector->size     = 0;
  vector->capacity = INITIAL_CAPACITY;

  return vector;
}

int vector_push_back(vector* vector, int item)
{
  int* vector_pointer;

  if ( vector->size + 1 == vector->capacity )
  {
    vector->capacity *= GROWTH_FACTOR;

    vector_pointer = realloc(vector->array, vector->capacity * sizeof(int));

    /* realloc failed... */
    if ( vector_pointer == NULL )
    {
      fprintf(stderr, "Unable to reallocate memory, exiting.\n");
      free(vector_pointer);
      return EXIT_FAILURE;
    }

    vector->array = vector_pointer;
  }

  /* push back item */
  vector->array[vector->size++] = item;

  return EXIT_SUCCESS;
}

int vector_remove_at(vector* vector, int index)
{
  // allocate an array with a size 1 less than the current one
  int* new_array = malloc((vector->size - 1) * sizeof(int));
  int element    = vector->array[index];

  // copy everything BEFORE the index
  if ( index != 0 ) memcpy(new_array, vector->array, index * sizeof(int));

  if ( index != (vector->size - 1) )
    memcpy(new_array + index, vector->array + index + 1,
           (vector->size - index - 1) * sizeof(int));

  vector->size--;
  vector->array = new_array;
  return element;
}

void vector_print(vector* vector)
{
  printf("vector: ");
  for ( int i = 0; i < vector->size; ++i )
    printf(i == vector->size - 1 ? "%d" : "%d ", vector->array[i]);

  printf("\n");
}

/* free vector */
void vector_free(vector* vector)
{
  free(vector->array);
  free(vector);
}

bool vector_is_empty(vector* vector) { return vector->size == 0; }

int vector_size(vector* vector) { return vector->size; }
