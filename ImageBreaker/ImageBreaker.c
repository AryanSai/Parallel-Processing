#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_FILE_SIZE 5000000

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char filename[100];
    FILE *image;
    long file_size;
    char *file_data = NULL, *received_data = NULL;

    if (world_rank == 0) // Process 0 reads the input image
    {
        snprintf(filename, sizeof(filename), "/home/dmacs/Desktop/MTech/103P/ImageBreaker/image.bmp");
        image = fopen(filename, "rb");

        if (image == NULL) // Check if the file exists
        {
            printf("Input image not found.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Determine the size of the BMP file
        fseek(image, 0, SEEK_END);
        file_size = ftell(image);
        fseek(image, 0, SEEK_SET);

        if (file_size > MAX_FILE_SIZE) // Check if the file size exceeds the maximum limit
        {
            printf("File size exceeds the maximum limit.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        file_data = (char *)malloc(file_size); // Allocate a buffer to store the file data

        fread(file_data, 1, file_size, image); // Read the file data into the buffer
        fclose(image);
    }

    // Broadcast the file size to all processes
    MPI_Bcast(&file_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);

    // Calculate the size of each portion for scattering
    long portion_size = file_size / world_size;

    // Allocate memory for received_data on all processes
    received_data = (char *)malloc(portion_size);

    // Scatter data from process 0 to all other processes
    MPI_Scatter(file_data, portion_size, MPI_CHAR, received_data, portion_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    printf("Scatter complete on process %d!\n", world_rank);

    // Create a filename for the scattered image portion
    snprintf(filename, sizeof(filename), "Portion%d.bmp", world_rank);
    FILE *output_part = fopen(filename, "wb");
    fwrite(received_data, 1, portion_size, output_part);
    fclose(output_part);

    // Gather the scattered image portions to process 0
    MPI_Gather(received_data, portion_size, MPI_CHAR, file_data, portion_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    printf("Gather complete on process %d!\n", world_rank);

    if (world_rank == 0)
    {
        // Process 0 saves
        FILE *output_image = fopen("Gathered_Image.bmp", "wb");
        fwrite(file_data, 1, file_size, output_image);
        fclose(output_image);

        printf("Output image saved.\n");

        // Free memory for the buffers on rank 0
        free(file_data);
    }

    // Free memory for received_data on all processes
    free(received_data);

    // Finalize MPI
    MPI_Finalize();

    return 0;
}