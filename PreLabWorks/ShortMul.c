#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define SIZE 10000

short int **randomMatrix()
{
    short int **matrix = (short int **)malloc(SIZE * sizeof(short int *)); // Allocate memory for all rows

    for (short int i = 0; i < SIZE; i++)
    {
        matrix[i] = (short int *)malloc(SIZE * sizeof(short int)); // Allocate memory for each row

        for (short int j = 0; j < SIZE; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }

    return matrix;
}

void printMatrix(short int **matrix)
{
    for (short int i = 0; i < SIZE; ++i)
    {
        printf("\n");
        for (short int j = 0; j < SIZE; ++j)
        {
            printf("%d   \t", matrix[i][j]);
        }
    }
}

short int main()
{
    srand(time(0));
    short int **product;
    short int i = 0, j = 0, k = 0;
    product = (short int **)malloc(SIZE * sizeof(short int *));
    short int **matrix1 = randomMatrix();
    printf("\nMatrix 1: ");
    // prshort intMatrix(matrix1);
    printf("\nMatrix 2: ");
    short int **matrix2 = randomMatrix();
    // prshort intMatrix(matrix2);
#pragma omp parallel for private(i, j, k) shared(matrix1, matrix2, product)
    for (i = 0; i < SIZE; i++)
    {
        product[i] = (short int *)malloc(SIZE * sizeof(short int));
        for (k = 0; k < SIZE; k++)
        {
            short int sum = 0;
#pragma omp parallel for reduction(+ : sum)
            for (j = 0; j < SIZE; j++)
            {
                sum += matrix1[i][k] * matrix2[k][j];
            }
            product[i][j] = sum;
        }
    }
    printf("\nProduct of Matrices:");
    // prshort intMatrix(product);
    printf("\nDone!");
    return 0;
}
