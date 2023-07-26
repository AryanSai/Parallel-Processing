#include <omp.h>
#include <stdio.h>

void main()
{
    double area, pi, x;
    int i, n;
    area = 0.0;
#pragma omp parallel for private(x)
    for (i = 0; i < n; i++)
    {
        x = (i + 0.5) / n;
#pragma omp critical
        area += 4.0 / (1.0 + x * x);
        // printf("%d ", omp_get_thread_num());
    }
    pi = area / n;
    printf("%f\n", pi);
}
