#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 20000

double omp_get_wtime(void);

int **randomMatrix()
{
    int **matrix = (int **)malloc(SIZE * sizeof(int *)); // Allocate memory for all rows

    for (int i = 0; i < SIZE; i++)
    {
        matrix[i] = (int *)malloc(SIZE * sizeof(int)); // Allocate memory for each row

        for (int j = 0; j < SIZE; j++)
        {
            matrix[i][j] = rand() % 100;
        }
    }

    return matrix;
}

void printMatrix(int **matrix)
{
    for (int i = 0; i < SIZE; ++i)
    {
        printf("\n");
        for (int j = 0; j < SIZE; ++j)
        {
            printf("%d   \t", matrix[i][j]);
        }
    }
}

int main()
{
    srand(time(0));
    double start;
    double end;
    int **matrixSum;
    int **matrix1 = randomMatrix();
    // printf("\nMatrix 1:");
    // printMatrix(matrix1);
    printf("\nMatrix 1 generated!!!");
    int **matrix2 = randomMatrix();
    // printf("\nMatrix 2:");
    // printMatrix(matrix2);
    printf("\nMatrix 2 generated!!!");

    matrixSum = (int **)malloc(SIZE * sizeof(int *));
    start = omp_get_wtime();
    // #pragma omp parallel for
    for (int i = 0; i < SIZE; ++i)
    {
        matrixSum[i] = (int *)malloc(SIZE * sizeof(int));
        for (int j = 0; j < SIZE; ++j)
        {
            matrixSum[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    end = omp_get_wtime();

    printf("\nMatrix Sum:");
    // printMatrix(matrixSum);
    printf("Time taken to execute in seconds : %f \n", end - start);
    return 0;
}
