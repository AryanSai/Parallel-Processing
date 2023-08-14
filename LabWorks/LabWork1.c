#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            {
                printf("Task 1 \n");
            }

            #pragma omp task
            {
                printf("Task 2 \n");
                #pragma omp taskyield // Yield execution to other tasks
            }

            #pragma omp task
            {
                printf("Task 3 \n");
            }

            #pragma omp taskwait
        }
    }

    return 0;
}
