// mpirun -np 2 ./a.out image.bmp
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_bmp_file>\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const char *input_file_name = argv[1];
    FILE *input_file = fopen(input_file_name, "rb");

    if (!input_file)
    {
        fprintf(stderr, "Error opening input BMP file.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Determine the size of the BMP file
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    // Allocate a buffer to store the file data
    char *file_data = (char *)malloc(file_size);

    // Read the file data into the buffer
    fread(file_data, 1, file_size, input_file);
    fclose(input_file);

    // MPI_Send(void* data,int count,MPI_Datatype datatype,int destination,int tag, MPI_Comm communicator)

    // Send the file data from the sender process to the receiver process
    if (world_rank == 0)
    {
        MPI_Send(file_data, file_size, MPI_CHAR, 3, 0, MPI_COMM_WORLD);
    }
    else if (world_rank == 3)
    {
        // Receiver process
        char *received_data = (char *)malloc(file_size);
        MPI_Recv(received_data, file_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Write the received data to a new BMP file
        FILE *output_file = fopen("received.bmp", "wb");
        fwrite(received_data, 1, file_size, output_file);
        fclose(output_file);

        free(received_data);
    }

    free(file_data);

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
