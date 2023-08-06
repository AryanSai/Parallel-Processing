#include <stdio.h>
#include <omp.h>

int main() {
    int n = 100000;
    int sum = 0;
    // OMP_DYNAMIC - Enable dynamic adjustment of threads
    omp_set_dynamic(1);
    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= n; i++) {
        printf("Thread %d handles iteration %d\n", omp_get_thread_num(), i);
        sum += i;
    }
    printf("Sum of numbers from 1 to %d: %d\n", n, sum);
    return 0;
}
