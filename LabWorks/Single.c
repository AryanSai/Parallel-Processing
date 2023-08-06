#include <stdio.h>
#include <omp.h>

int main() {
    int sum = 0;

    #pragma omp parallel for private(sum)
    for (int i = 1; i <= 10; i++) {
        sum += i;
        printf("Thread %d: sum = %d \n", omp_get_thread_num(), sum);
    }

    return 0;
}
