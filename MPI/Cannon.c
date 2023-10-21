#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MATRIX_SIZE 3 // Define the size of the matrices

void generateRandomMatrix(float *matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i * size + j] = (float)rand() / RAND_MAX; // Generate random float values between 0 and 1
        }
    }
}

void matrixMultiply(float *A, float *B, float *C, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            C[i * size + j] = 0;
            for (int k = 0; k < size; k++)
            {
                C[i * size + j] += A[i * size + k] * B[k * size + j];
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int rank, size;
    float *A, *B, *C;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL)); // Seed the random number generator

    int matrixSize = MATRIX_SIZE;

    if (rank == 0)
    {
        float *A = (float *)malloc(matrixSize * matrixSize * sizeof(float));
        float *B = (float *)malloc(matrixSize * matrixSize * sizeof(float));
        float *C = (float *)malloc(matrixSize * matrixSize * sizeof(float));

        // Generate random matrices A and B
        generateRandomMatrix(A, matrixSize);
        generateRandomMatrix(B, matrixSize);

        // Display the random matrices
        printf("Matrix A:\n");
        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                printf("%f\t", A[i * matrixSize + j]);
            }
            printf("\n");
        }

        printf("\nMatrix B:\n");
        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                printf("%f\t", B[i * matrixSize + j]);
            }
            printf("\n");
        }

        // Scatter A and B to all processes
        MPI_Bcast(A, matrixSize * matrixSize, MPI_FLOAT, 0, MPI_COMM_WORLD);
        MPI_Bcast(B, matrixSize * matrixSize, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }

    // Create local matrices A and B for each process
    float *localA = (float *)malloc((matrixSize / size) * matrixSize * sizeof(float));
    float *localB = (float *)malloc((matrixSize / size) * matrixSize * sizeof(float));

    // Scatter A and B to all processes
    MPI_Scatter(A, (matrixSize / size) * matrixSize, MPI_FLOAT, localA, (matrixSize / size) * matrixSize, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, (matrixSize / size) * matrixSize, MPI_FLOAT, localB, (matrixSize / size) * matrixSize, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Perform matrix multiplication
    float *localC = (float *)malloc((matrixSize / size) * matrixSize * sizeof(float));
    matrixMultiply(localA, localB, localC, matrixSize / size);

    // Gather results from all processes
    MPI_Gather(localC, (matrixSize / size) * matrixSize, MPI_FLOAT, C, (matrixSize / size) * matrixSize, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        // Display the result matrix C
        printf("\nResult Matrix C:\n");
        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                printf("%f\t", C[i * matrixSize + j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}
