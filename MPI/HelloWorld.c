// pass the number of proccesses we want in the world as parameters when running

// mpicc MPI.c
// mpirun -np 4 ./a.out
//-np #     Run  this  many  copies of the program on the given nodes.

#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the running process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Print off a hello world message
    printf("Hello world from processor rank %d out of %d processors\n", world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
}
