#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define SIZE 10

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

int *randomVector()
{
    int *vector = (int *)malloc(SIZE * sizeof(int));
    for (int j = 0; j < SIZE; j++)
    {
        vector[j] = rand() % 10;
    }
    return vector;
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

void printVector(int *vector)
{

    for (int j = 0; j < SIZE; ++j)
    {
        printf("%d   \t", vector[j]);
    }
}

int main()
{
    srand(time(0));
    int **product;
    int i = 0, j = 0, k = 0;
    product = (int **)malloc(SIZE * sizeof(int *));
    int **matrix1 = randomMatrix();
    printf("\nMatrix: ");
    // printMatrix(matrix1);

    printf("\nVector: ");
    int *vector = randomVector();
    printVector(vector);

    printf("\nProduct of Matrices:");
    // printMatrix(product);
    printf("\nDone!");
    return 0;
}
