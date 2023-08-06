#include <stdio.h>
#include <omp.h>

#define SIZE 10

int main() {
    int arr[SIZE];

    // Initialize the array with values from 1 to SIZE
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1;
    }

    #pragma omp parallel for ordered
    for (int i = 0; i < SIZE; i++) {
        // Print the value of each array element in the original order
        printf("Array element %d: %d\n", i, arr[i]);
    }

    return 0;
}
