#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 500
#define CACHE_STEP 64

// Function to access the array sequentially to measure access time
long access_array(int* array, int size) {
    int i;
    long sum = 0;
    for (i = 0; i < size; i += CACHE_STEP) {
        sum += array[i % ARRAY_SIZE]; // Access within bounds of the array
    }
    return sum;
}

int main() {
    int array[ARRAY_SIZE];
    int i;

    // Initialize the array
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
    }

    // Measure the access time
    clock_t start_time = clock();
    long sum = access_array(array, ARRAY_SIZE);
    clock_t end_time = clock();

    // Calculate elapsed time in milliseconds
    double elapsed_time = (double)(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC;

    printf("Sum: %ld\n", sum);
    printf("Elapsed Time: %.2f ms\n", elapsed_time);

    // Estimate the cache size based on the access time
    int cache_size = CACHE_STEP;
    while (elapsed_time < 1.0) {
        cache_size += CACHE_STEP;
        start_time = clock();
        sum = access_array(array, cache_size);
        end_time = clock();
        elapsed_time = (double)(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC;
    }

    printf("Estimated L1 Cache Size: %d bytes\n", cache_size);

    return 0;
}
