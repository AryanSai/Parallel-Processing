#include <stdio.h>
#include <omp.h>

#define ARRAY_SIZE 10

int main()
{
    int arr[ARRAY_SIZE];
    int total_sum = 0;

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        arr[i] = i + 1;
    }

    // The 'sum' variable will be private to each thread and initialized with 0
    int sum = 0;

#pragma omp parallel firstprivate(sum)
    {
// Each thread will have its private copy of 'sum', initialized with 0
#pragma omp for
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            printf("Sum Value:  %d\n", sum);
            sum += arr[i]; // Each thread has its own 'sum'
            printf("Sum of array elements is %d in thread  %d\n", sum, omp_get_thread_num());
        }

// Accumulate the private 'sum' values into the 'total_sum'
#pragma omp critical
        total_sum += sum;
    }

    printf("Total Sum of array elements: %d\n", total_sum);

    return 0;
}
