#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 1000000

int *randomVector()
{
  int *arr = (int *)malloc(SIZE * sizeof(int));

  for (int i = 0; i < SIZE; i++)
  {
    arr[i] = rand() % 100;
  }

  return arr;
}

void printVector(int *vector)
{
  printf("Vector: \n");
  for (int i = 0; i < SIZE; i++)
  {
    printf("%d ", vector[i]);
  }
  printf("\n\n");
}

int main()
{
  srand(time(NULL)); // ensures that different random numbers are generated in each program run
  double start;
  double end;
  int *vector1 = randomVector();
  printf("Vector 1 generated\n");
  int *vector2 = randomVector();
  printf("Vector 2 generated\n");
  // printVector(vector1);
  // printVector(vector2);
  int result[SIZE];

  for (int i = 0; i < SIZE; i++)
  {
    // int tid = omp_get_thread_num();
    // printf("Hello World... from thread  %d !\n", tid);
    result[i] = vector1[i] + vector2[i];
  }
  printf("Vector Sum generated\n");
  // printf("Resultant Vector: \n");
  // printVector(result);
  return 0;
}