#include <stdio.h>
#include <omp.h>
#define ARRAY_SIZE 4

int main()
{
    int arr[ARRAY_SIZE];
    int total_sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        arr[i] = i + 1;
    }
    // The 'sum' variable will be private to each thread
    int sum = 0;
#pragma omp parallel for private(sum)

    // Each thread will have its private copy of 'sum'
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("Sum Value:  %d\n", sum);
        sum += arr[i]; // Each thread has its own 'sum'
        printf("Sum of array elements is %d in thread  %d\n", sum, omp_get_thread_num());
    }
    printf("Sum of array elements is %d\n", sum);

    return 0;
}
