
// command to run ===> gcc -fopenmp HelloWorld.c
// export OMP_NUM_THREADS=8
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int tid;
#pragma omp parallel private(tid) num_threads(5) // each thread has its own private copy
    {
        tid = omp_get_thread_num();
        printf("Hello World... from thread  %d !\n", tid);

        if (tid == 0)
        { // parent
            printf("I am the parent!!\n");
            printf("Number of threads = %d\n", omp_get_num_threads());
        }
    }
}