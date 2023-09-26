#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Define the number of rows and columns
    int num_rows = 2; // Adjust this to the actual number of rows
    int num_cols = 2; // Adjust this to the actual number of columns

    if (world_rank == 0)
    {
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                // Generate a filename based on the position
                char filename[100];
                snprintf(filename, sizeof(filename), "output_%d_%d.bmp", i, j);
                FILE *image = fopen(filename, "rb");

                // Determine the size of the BMP file
                fseek(image, 0, SEEK_END);
                long file_size = ftell(image);
                fseek(image, 0, SEEK_SET);

                // Allocate a buffer to store the file data
                char *file_data = (char *)malloc(file_size);

                // Read the file data into the buffer
                fread(file_data, 1, file_size, image);
                fclose(image);

                // Send the file data to the corresponding process
                MPI_Send(file_data, file_size, MPI_CHAR, (i * num_cols + j) % (world_size - 1) + 1, 0, MPI_COMM_WORLD);

                free(file_data);
            }
        }
    }
    else
    {
        // Receiver process
        MPI_Status status;
        int source_rank = world_rank;

        // Receive the file data from the corresponding process
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        int file_size;
        MPI_Get_count(&status, MPI_CHAR, &file_size);
        char *received_data = (char *)malloc(file_size);
        MPI_Recv(received_data, file_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Generate a filename for the received file
        char received_filename[100];
        // snprintf(received_filename, sizeof(received_filename), "received_%d_%d.bmp", source_rank / num_cols, source_rank % num_cols);
        snprintf(received_filename, sizeof(received_filename), "received_%d_%d.bmp",
                 source_rank / num_cols, source_rank % num_cols);
        if (source_rank / num_cols >= num_rows || source_rank % num_cols >= num_cols)
        {
            snprintf(received_filename, sizeof(received_filename), "received_0_0.bmp");
        }
        FILE *output_file = fopen(received_filename, "wb");
        fwrite(received_data, 1, file_size, output_file);
        fclose(output_file);

        free(received_data);
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
