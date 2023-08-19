#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define SIZE 1000

int **randomMatrix()
{
    int **matrix = (int **)malloc(SIZE * sizeof(int *)); // Allocate memory for all rows

    for (int i = 0; i < SIZE; i++)
    {
        matrix[i] = (int *)malloc(SIZE * sizeof(int)); // Allocate memory for each row

        for (int j = 0; j < SIZE; j++)
        {
            matrix[i][j] = rand() % 10;
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
    int **product;
    int i = 0, j = 0, k = 0;
    product = (int **)malloc(SIZE * sizeof(int *));
    int **matrix1 = randomMatrix();
    printf("\nMatrix 1: ");
    // printMatrix(matrix1);
    printf("\nMatrix 2: ");
    int **matrix2 = randomMatrix();
    // printMatrix(matrix2);
#pragma omp parallel for private(i, j, k) shared(matrix1, matrix2, product)
    for (k = 0; k < SIZE; k++)
    {
        for (j = 0; j < SIZE; j++)
        {
            int sum = 0;
#pragma omp parallel for reduction(+ : sum)
            for (i = 0; i < SIZE; i++)
            {
                product[i] = (int *)malloc(SIZE * sizeof(int));
                sum += matrix1[i][k] * matrix2[k][j];
            }
            product[i][j] = sum;
        }
    }
    printf("\nProduct of Matrices:");
    // printMatrix(product);
    printf("\nDone!");
    return 0;
}
