#include <stdio.h>
#include <omp.h>

#define SIZE 4

int main() {
    int matrix[SIZE][SIZE];
    int sum = 0;

    // Initialize the matrix with some values
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = i * SIZE + j + 1;
        }
    }

    #pragma omp parallel for collapse(2) reduction(+:sum)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            sum += matrix[i][j];
        }
    }

    printf("Sum of all elements in the matrix: %d\n", sum);

    return 0;
}
