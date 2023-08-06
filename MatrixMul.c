#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define SIZE 50000

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
    printf("%d \n", SIZE);
    srand(time(0));
    int **product;
    int i = 0, j = 0;
    product = (int **)malloc(SIZE * sizeof(int *));
    int **matrix1 = randomMatrix();
    int **matrix2 = randomMatrix();
// #pragma omp parallel for private(j)
    for (i = 0; i < SIZE; i++)
    {
        product[i] = (int *)malloc(SIZE * sizeof(int));
        for (j = 0; j < SIZE; ++j)
        {
            product[i][j] = matrix1[i][j] * matrix2[j][i];
        }
    }
    // printMatrix(product);
    printf("Done!");
    return 0;
}
