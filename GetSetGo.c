#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
    int num, i;
    num = omp_get_num_procs();
    printf("Number of processors: %d\n", num);
    omp_set_num_threads(1);
#pragma omp parallel
    printf("Thread Number: %d\n", omp_get_num_threads());
    for (i = 0; i < 1000000;)
    {
        i = i + 1;
    }
    printf("Result: %d\n", i);
}