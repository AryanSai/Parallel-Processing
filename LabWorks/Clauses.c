#include <stdio.h>
#include <omp.h>

int value = 42;

// Declare 'value' as threadprivate
#pragma omp threadprivate(value)

int main()
{

#pragma omp parallel
    {
#pragma omp single copyprivate(value)
        {
            value += omp_get_thread_num();
            printf("Thread %d: value = %d\n", omp_get_thread_num(), value);
        }
    }

    return 0;
}
