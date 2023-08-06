#include <stdio.h>
#include <omp.h>

int main()
{
    int total_sum = 0;
    int i;

#pragma omp parallel for reduction(+ : total_sum)
    for (i = 1; i <= 10; i++)
    {
        total_sum += i;
    }

    // The value of 'i' after the parallel region will be the last value calculated in the loop
    printf("Last value of 'i' after the loop: %d\n", i);

    printf("Total Sum of numbers 1 to 10: %d\n", total_sum);

    return 0;
}
