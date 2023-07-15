#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 100000

double omp_get_wtime(void);

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
  srand(time(NULL)); //ensures that different random numbers are generated in each program run
  double start;
  double end;
  int *vector1 = randomVector();
  int *vector2 = randomVector();
  // printVector(vector1);
  // printVector(vector2);
  int result[SIZE];
  
  start = omp_get_wtime();
#pragma omp parallel for
  for (int i = 0; i < SIZE; i++)
  {
    result[i] = vector1[i] + vector2[i];
  }
  end = omp_get_wtime();

  printf("Resultant Vector: \n");
  // printVector(result);
  printf("Time taken to execute in seconds : %f \n", end - start);
  return 0;
}