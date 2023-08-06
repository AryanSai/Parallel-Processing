#include <stdio.h>
#include <omp.h>

#define SIZE 8

int main() {
    int arr[SIZE];
    int sum = 0;

    // Initialize the array with values from 1 to SIZE
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1;
    }

    #pragma omp parallel for nowait
    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }

    // The following statement will be executed without waiting for the parallel loop to finish
    printf("Sum of array elements: %d\n", sum);

    return 0;
}
