#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            printf("Usage: %s <N>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int N = atoi(argv[1]);
    int sqrtN = (int)sqrt(N);
    int n, i, j;

    // Determine local range
    int local_start = (rank * (N - 1) / size) + 2;
    int local_end = ((rank + 1) * (N - 1) / size) + 2;
    if (local_end > N)
        local_end = N;

    // Create an array for the local prime numbers
    int* local_primes = (int*)malloc((local_end - local_start + 1) * sizeof(int));
    for (i = local_start, n = 0; i <= local_end; i++, n++) {
        local_primes[n] = 1; // Assume all numbers are prime initially
    }

    // Sieve of Eratosthenes
    for (i = 2; i <= sqrtN; i++) {
        if (i <= local_end) {
            if (local_primes[i - local_start]) {
                for (j = i * i; j <= local_end; j += i) {
                    local_primes[j - local_start] = 0;
                }
            }
        }

        // Synchronize to ensure that all processes have the updated local_primes array
        MPI_Barrier(MPI_COMM_WORLD);

        // Broadcast the prime number being considered to all processes
        MPI_Bcast(&i, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Broadcast the local_primes array to all processes
        MPI_Bcast(local_primes, local_end - local_start + 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        printf("Prime numbers up to %d: 2 ", N);
        for (i = 0; i < size; i++) {
            for (j = local_start; j <= local_end; j++) {
                if (local_primes[j - local_start]) {
                    printf("%d ", j);
                }
            }
            if (i < size - 1) {
                MPI_Recv(local_primes, local_end - local_start + 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        printf("\n");
    } else {
        MPI_Send(local_primes, local_end - local_start + 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    free(local_primes);
    MPI_Finalize();

    return 0;
}
