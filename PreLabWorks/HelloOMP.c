// command to run ===> gcc -fopenmp HelloWorld.c
// export OMP_NUM_THREADS=8
#include <omp.h>
#include <stdio.h>
#include <time.h>

int main()
{
    clock_t start, end;
    double duration;
    start = clock();
    #pragma omp parallel // each thread has its own private copy
    {
        // printf("%ld", start);
        for (int j = 0; j < 1000000; j++)
        {
            int tid = omp_get_thread_num();
            // printf("Hello World... from thread  %d !\n", tid);
        }
    }
    end = clock();
    // printf("%ld \n", end);
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken  %f !\n", duration);
}