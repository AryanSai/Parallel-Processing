#include <omp.h>
#include <stdio.h>

void main()
{
    int i, j, n = 100, a[100][100];
#pragma omp parallel for private(j) schedule(static, 1)
    for (i = 0; i < n; i++)
        for (j = i; j < n; j++)
            a[i][j] = i + j;
}
