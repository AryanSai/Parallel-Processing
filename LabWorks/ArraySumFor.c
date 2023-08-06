#include <stdio.h>
#include <omp.h>

#define ARRAY_SIZE 10

int main()
{
    int arr[ARRAY_SIZE];
    int sum = 0;
    int i;

    // Initialize the array with values from 1 to ARRAY_SIZE
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        arr[i] = i + 1;
    }

#pragma omp parallel for reduction(+ : sum) schedule(dynamic, 2)
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        printf("The value of i: %d\n", i);
        sum += arr[i];
    }

    printf("Sum of array elements: %d\n", sum);

    return 0;
}
