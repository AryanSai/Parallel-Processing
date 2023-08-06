#include <stdio.h>
#include <omp.h>

int main() {
    int n = 8;
    int sum = 0;

    // OMP_NUM_THREADS - Set the number of threads to be used as 4
    omp_set_num_threads(4);

    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= n; i++) {
        printf("Thread %d handles iteration %d\n", omp_get_thread_num(), i);
        sum += i;
    }
    printf("Sum of numbers from 1 to %d: %d\n", n, sum);

    return 0;
}
