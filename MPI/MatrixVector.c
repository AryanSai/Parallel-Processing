#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_SIZE 4

void matrixVectorMultiply(float *matrix, float *vector, float *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i * cols + j] * vector[j];
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int matrixSize = MATRIX_SIZE;
    int localRows = matrixSize / size;

    float *matrix = NULL;
    float *vector = NULL;
    float *localMatrix = (float *)malloc(localRows * matrixSize * sizeof(float));
    float *localVector = (float *)malloc(matrixSize * sizeof(float));
    float *localResult = (float *)malloc(localRows * sizeof(float));

    if (rank == 0) {
        matrix = (float *)malloc(matrixSize * matrixSize * sizeof(float));
        vector = (float *)malloc(matrixSize * sizeof(float));

        // Initialize the matrix and vector (for demonstration purposes)
        for (int i = 0; i < matrixSize; i++) {
            vector[i] = 1.0; // Initialize the vector with 1.0
            for (int j = 0; j < matrixSize; j++) {
                matrix[i * matrixSize + j] = i * matrixSize + j; // Initialize the matrix with values
            }
        }
    }

    // Scatter the matrix and broadcast the vector to all processes
    MPI_Scatter(matrix, localRows * matrixSize, MPI_FLOAT, localMatrix, localRows * matrixSize, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, matrixSize, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Perform matrix-vector multiplication locally
    matrixVectorMultiply(localMatrix, vector, localResult, localRows, matrixSize);

    // Gather results from all processes
    float *finalResult = (float *)malloc(matrixSize * sizeof(float));
    MPI_Gather(localResult, localRows, MPI_FLOAT, finalResult, localRows, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Display the final result
        printf("Matrix-Vector Multiplication Result:\n");
        for (int i = 0; i < matrixSize; i++) {
            printf("%f\n", finalResult[i]);
        }
    }

    if (rank == 0) {
        free(matrix);
        free(vector);
    }
    free(localMatrix);
    free(localVector);
    free(localResult);
    free(finalResult);

    MPI_Finalize();

    return 0;
}
