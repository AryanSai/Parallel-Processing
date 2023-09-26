#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> //working with integer types
#include <mpi.h>
#include <string.h>

#define ROWS 2
#define COLS 2

long file_size;

struct Image
{
    int width;
    int height;
    int size;
    uint8_t *imageData;
};

// This function is responsible for saving a portion of the original image as a new BMP file.
void saveSubImage(uint8_t *imageData, int width, int height, int x, int y, int subWidth, int subHeight, const char *filename)
{
    FILE *outputImage = fopen(filename, "wb");
    if (outputImage == NULL)
    {
        printf("Error opening %s for writing.\n", filename);
        return;
    }

    uint8_t bmpHeader[14] = {
        0x42, 0x4D, // "BM" identifier
        0, 0, 0, 0, // File size (set later)
        0, 0, 0, 0, // Reserved
        54, 0, 0, 0 // Offset to image data (54 bytes)
    };

    // BMP info header (40 bytes)
    uint8_t bmpInfoHeader[40] = {
        40, 0, 0, 0, // Info header size (40 bytes)
        0, 0, 0, 0,  // Image width (set later)
        0, 0, 0, 0,  // Image height (set later)
        1, 0,        // Number of color planes (1)
        24, 0,       // Bits per pixel (24, for 3 channels)
        0, 0, 0, 0,  // Compression method (0, no compression)
        0, 0, 0, 0,  // Image size (set later)
        0, 0, 0, 0,  // Horizontal resolution (pixels per meter, not specified)
        0, 0, 0, 0,  // Vertical resolution (pixels per meter, not specified)
        0, 0, 0, 0,  // Number of colors in palette (0, no palette)
        0, 0, 0, 0   // Number of important colors (0, all are important)
    };

    int rowSize = ((subWidth * 3 + 3) / 4) * 4; // Each row is a multiple of 4 bytes

    // Update file size, image width, image height, and image size in the header
    bmpHeader[2] = (uint8_t)(54 + subHeight * rowSize); // File size
    bmpInfoHeader[4] = (uint8_t)(subWidth);             // Image width
    bmpInfoHeader[5] = (uint8_t)(subWidth >> 8);
    bmpInfoHeader[6] = (uint8_t)(subWidth >> 16);
    bmpInfoHeader[7] = (uint8_t)(subWidth >> 24);
    bmpInfoHeader[8] = (uint8_t)(subHeight); // Image height
    bmpInfoHeader[9] = (uint8_t)(subHeight >> 8);
    bmpInfoHeader[10] = (uint8_t)(subHeight >> 16);
    bmpInfoHeader[11] = (uint8_t)(subHeight >> 24);
    bmpInfoHeader[20] = (uint8_t)(subHeight * rowSize); // Image size

    // Write the headers
    fwrite(bmpHeader, 1, 14, outputImage);
    fwrite(bmpInfoHeader, 1, 40, outputImage);

    // Write the image data
    for (int i = y; i < y + subHeight; i++)
    {
        fwrite(imageData + (i * width + x) * 3, 1, subWidth * 3, outputImage);
        // Pad rows to be multiples of 4 bytes
        for (int j = 0; j < (4 - (subWidth * 3) % 4) % 4; j++)
        {
            fputc(0, outputImage);
        }
    }

    fclose(outputImage);
}

// Function to save an image as a BMP file
void saveImageAsBMP(uint8_t *imageData, int width, int height, const char *filename)
{
    FILE *outputImage = fopen(filename, "wb");
    if (outputImage == NULL)
    {
        printf("Error opening %s for writing.\n", filename);
        return;
    }

    // BMP header (14 bytes)
    uint8_t bmpHeader[14] = {
        0x42, 0x4D, // "BM" identifier
        0, 0, 0, 0, // File size (set later)
        0, 0, 0, 0, // Reserved
        54, 0, 0, 0 // Offset to image data (54 bytes)
    };

    // BMP info header (40 bytes)
    uint8_t bmpInfoHeader[40] = {
        40, 0, 0, 0, // Info header size (40 bytes)
        0, 0, 0, 0,  // Image width (set later)
        0, 0, 0, 0,  // Image height (set later)
        1, 0,        // Number of color planes (1)
        24, 0,       // Bits per pixel (24, for 3 channels)
        0, 0, 0, 0,  // Compression method (0, no compression)
        0, 0, 0, 0,  // Image size (set later)
        0, 0, 0, 0,  // Horizontal resolution (pixels per meter, not specified)
        0, 0, 0, 0,  // Vertical resolution (pixels per meter, not specified)
        0, 0, 0, 0,  // Number of colors in palette (0, no palette)
        0, 0, 0, 0   // Number of important colors (0, all are important)
    };

    int rowSize = ((width * 3 + 3) / 4) * 4; // Each row is a multiple of 4 bytes

    // Update file size, image width, image height, and image size in the header
    bmpHeader[2] = (uint8_t)(54 + height * rowSize); // File size
    bmpInfoHeader[4] = (uint8_t)(width);             // Image width
    bmpInfoHeader[5] = (uint8_t)(width >> 8);
    bmpInfoHeader[6] = (uint8_t)(width >> 16);
    bmpInfoHeader[7] = (uint8_t)(width >> 24);
    bmpInfoHeader[8] = (uint8_t)(height); // Image height
    bmpInfoHeader[9] = (uint8_t)(height >> 8);
    bmpInfoHeader[10] = (uint8_t)(height >> 16);
    bmpInfoHeader[11] = (uint8_t)(height >> 24);
    bmpInfoHeader[20] = (uint8_t)(height * rowSize); // Image size

    // Write the headers
    fwrite(bmpHeader, 1, 14, outputImage);
    fwrite(bmpInfoHeader, 1, 40, outputImage);

    // Write the image data
    for (int i = height - 1; i >= 0; i--)
    {
        fwrite(imageData + i * width * 3, 1, width * 3, outputImage);
        // Pad rows to be multiples of 4 bytes
        for (int j = 0; j < (4 - (width * 3) % 4) % 4; j++)
        {
            fputc(0, outputImage);
        }
    }

    fclose(outputImage);
}

void receiveImagebyProcesses()
{
    // Receiver process
    MPI_Status status;
    char received_filename[100]; // Receive the file name
    MPI_Recv(received_filename, sizeof(received_filename), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

    MPI_Recv(&file_size, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD, &status); // Receive the file size

    char *received_data = (char *)malloc(file_size); // Receive the file data
    MPI_Recv(received_data, file_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Write the received data to a new BMP file with the received file name
    FILE *output_file = fopen(received_filename, "wb");
    fwrite(received_data, 1, file_size, output_file);
    fclose(output_file);

    free(received_data);
}

struct Image readImage()
{
    struct Image image;

    FILE *inputImage = fopen("/home/dmacs/Desktop/MTech/103P/image.bmp", "rb");
    if (inputImage == NULL)
    {
        printf("Error opening the input image.\n");
        exit;
    }

    uint8_t bmpHeader[54]; // BMP header is typically 54 bytes in size for standard BMP files
    if (fread(bmpHeader, 1, 54, inputImage) != 54 || bmpHeader[0] != 'B' || bmpHeader[1] != 'M')
    { // verify that the file is a valid BMP file
        printf("Invalid BMP file.\n");
        fclose(inputImage);
        exit;
    }

    image.width = *((int *)&bmpHeader[18]);
    image.height = *((int *)&bmpHeader[22]);
    image.size = image.width * image.height * 3; // Calculate total image size in bytes

    // Allocate memory for imageData
    image.imageData = (uint8_t *)malloc(image.size);
    if (image.imageData == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(inputImage);
        exit;
    }

    // It reads the image data from the input BMP file into the allocated memory.
    //  If there is an error reading the data, it prints an error message and exits with an error code.
    if (fread(image.imageData, 1, image.size, inputImage) != image.size)
    {
        printf("Error reading image data.\n");
        fclose(inputImage);
    }
    return image;
}

struct Image breakImage(struct Image image)
{
    int small_height = image.height / ROWS;
    int small_width = image.width / COLS;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            // Calculate the coordinates for the top-left corner of the small image
            int x = j * small_width;
            int y = i * small_height;

            // Generate a filename based on the position
            char filename[100];
            snprintf(filename, sizeof(filename), "output_%d_%d.bmp", i, j);

            // Save the small image portion
            saveSubImage(image.imageData, image.width, image.height, x, y, small_width, small_height, filename);

            printf("Saved %s\n", filename);
        }
    }
}

void *reconstructImage(int num_rows, int num_cols, int small_width, int small_height)
{
    // Calculate the dimensions of the original image
    int width = num_cols * small_width;
    int height = num_rows * small_height;

    // Calculate the total image size in bytes
    int imageSize = width * height * 3; // Assuming 24-bit color (3 bytes per pixel)

    // Allocate memory for the reconstructed image
    uint8_t *reconstructedImageData = (uint8_t *)malloc(imageSize);

    if (reconstructedImageData == NULL)
    {
        printf("Memory allocation error for reconstructed image.\n");
        return NULL;
    }

    // Initialize the reconstructed image to all zeros
    for (int i = 0; i < imageSize; i++)
    {
        reconstructedImageData[i] = 0;
    }

    // Iterate through the broken images and copy their data to the reconstructed image
    for (int j = num_cols - 1; j >= 0; j--)
    {
        for (int i = num_rows - 1; i >= 0; i--)
        {
            // Calculate the coordinates for the top-left corner of the small image
            int x = i * small_width;
            int y = j * small_height;

            // Generate the filename for the broken image
            char filename[100];
            snprintf(filename, sizeof(filename), "output_%d_%d.bmp", j, i);

            // Open the broken image file for reading
            FILE *brokenImage = fopen(filename, "rb");
            if (brokenImage == NULL)
            {
                printf("Error opening %s for reading.\n", filename);
                free(reconstructedImageData);
                return NULL;
            }

            // Skip the BMP header (first 54 bytes)
            fseek(brokenImage, 54, SEEK_SET);

            // Read and copy the pixel data from the broken image to the reconstructed image row by row
            for (int h = 0; h < small_height; h++)
            {
                fread(reconstructedImageData + ((y + h) * width + x) * 3, 1, small_width * 3, brokenImage);

                // Pad rows to be multiples of 4 bytes in the broken image
                for (int pad = 0; pad < (4 - (small_width * 3) % 4) % 4; pad++)
                {
                    fgetc(brokenImage);
                }
            }

            // Close the broken image file
            fclose(brokenImage);
        }
    }
    if (reconstructedImageData != NULL)
    {
        // Define the filename for the reconstructed image
        const char *reconstructedFilename = "reconstructed_image.bmp";

        // Save the reconstructed image as a BMP file
        saveImageAsBMP(reconstructedImageData, ROWS * small_width, COLS * small_height, reconstructedFilename);

        printf("Reconstructed image saved as %s\n", reconstructedFilename);

        // Free the memory allocated for the reconstructed image
        free(reconstructedImageData);
    }
}
int main()
{
    // setup MPI
    MPI_Init(NULL, NULL);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    struct Image image;
    image = readImage();

    if (world_rank == 0)
    {
        breakImage(image);

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
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

                // Send the file name and file data to the corresponding process
                MPI_Send(filename, strlen(filename) + 1, MPI_CHAR, (i * ROWS + j) % (world_size - 1) + 1, 0, MPI_COMM_WORLD);
                MPI_Send(&file_size, 1, MPI_LONG, (i * COLS + j) % (world_size - 1) + 1, 0, MPI_COMM_WORLD); // Send file size
                MPI_Send(file_data, file_size, MPI_CHAR, (i * COLS + j) % (world_size - 1) + 1, 0, MPI_COMM_WORLD);

                free(file_data);
            }
        }
    }
    else
    {
        receiveImagebyProcesses();
    }

    if (world_rank == 0)
    {
        // Reconstruct the original image
        reconstructImage(ROWS, COLS, image.width / COLS, image.height / ROWS);
    }

    MPI_Finalize();
    return 0;
}