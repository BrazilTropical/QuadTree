#include <stdio.h>
#include "quadtree.h"
#include "queue.h"
#include <stdlib.h>
#include <math.h>

#define DEBUG 0
#define EPSILON 2
#define MAX_INT 1000000

#define MATRIX_SIZE 8
float idistance(float x1, float y1, float x2, float y2)
{
  return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}
void stress_test()
{
  #define SIZE 32
  int matrix[SIZE][SIZE];
  int epsilon = 0;


  for(int i = 0; i < SIZE; ++i)
  {
    for(int j = 0; j < SIZE; ++j)
    {
      if( i <= j )
      {
       matrix[i][j] = j;
      printf("%3d ", j);
      }
      else
      {

       matrix[i][j] = i;
      printf("%3d ", i);
      }
    }
    printf("\n");
  }

  quad_tree* qt = new_quad_tree(SIZE, SIZE);

  qt = split_tree(qt, SIZE, matrix, epsilon);

  qt = merge_tree(qt, SIZE, epsilon);

  destroy_tree(qt);


}

void test()
{
   int matrix[MATRIX_SIZE][MATRIX_SIZE] =
  {
    {11, 12, 10, 11, 12, 10, 12, 11},
    {11, 11, 12, 10, 10, 11, 06, 05},
    {10, 12, 12, 11, 12, 07, 06 ,06},
    {12, 12, 11, 11, 06, 07, 07, 07},
    {11, 10, 05, 06, 06, 06, 06, 07},
    {12, 12, 04, 06, 05, 05, 05, 05},
    {11, 05, 06, 06, 07, 07, 05, 07},
    {06, 05, 06, 06, 07, 06, 05, 05}
  };
  int matrix2[4][4] =
    {
    {1,2,3,4},
    {1,10,10,10},
    {1,10,10,4},
    {1,2,3,4}
    };


  quad_tree* qt = new_quad_tree(MATRIX_SIZE, MATRIX_SIZE);

  printf("Matrix\n");
  for(int i = 0; i < MATRIX_SIZE; ++i)
  {
    for(int j = 0; j < MATRIX_SIZE; ++j)
    {
      printf("%3d ", matrix[i][j]);
    }
    printf("\n");
  }

  printf("-----------------------------------------------\n");

  qt = split_tree(qt, MATRIX_SIZE, matrix, EPSILON);

  qt = merge_tree(qt, MATRIX_SIZE, EPSILON);

  destroy_tree(qt);


}

int main()
{
  //stress_test();
  test();
   return 0;
}
