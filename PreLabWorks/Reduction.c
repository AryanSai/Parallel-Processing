#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double func(int i)
{
    return i;
}

void main()
{
    int i;
    double ZZ, func(), res = 0.0;
#pragma omp parallel for reduction(+ : res) private(ZZ)

    for (i = 0; i < 1000; i++)
    {
        ZZ = func(i);
        printf("Result: %f\n", ZZ);
        res = res + ZZ;
    }
    printf("Result: %f\n", res);
}