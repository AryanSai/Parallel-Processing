#include <stdio.h>
#include <omp.h>
// Global variable declared as threadprivate
int x;
// Declare a threadprivate variable
#pragma omp threadprivate(x)

int main()
{
    // Initialize the threadprivate variable with the thread number
    x = 0;

#pragma omp parallel num_threads(4)
    {
        int thread_id = omp_get_thread_num();
        // Each thread will have its own copy of x
        // The value of x will be specific to each thread
        x = thread_id + 1;
        printf("Thread %d: x = %d\n", thread_id, x);
    }

    // The value of x outside the parallel region will be the value of the last thread's copy
    // In this case, it will be the value of x from the last thread (thread 0) that executed the parallel region.
    printf("Value of x after the parallel region: %d\n", x);

    return 0;
}
