#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mpi.h>

// This function is responsible for saving a portion of the original image as a new BMP file.
void saveSubImage(uint8_t *imageData, int width, int height, int x, int y, int subWidth, int subHeight, const char *filename)
{
    FILE *outputImage = fopen(filename, "wb");
    if (outputImage == NULL)
    {
        printf("Error opening %s for writing.\n", filename);
        return;
    }

    uint8_t bmpHeader[14] = {0x42, 0x4D, 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    uint8_t bmpInfoHeader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int rowSize = ((subWidth * 3 + 3) / 4) * 4;
    bmpHeader[2] = (uint8_t)(54 + subHeight * rowSize);
    bmpInfoHeader[4] = (uint8_t)(subWidth);
    bmpInfoHeader[5] = (uint8_t)(subWidth >> 8);
    bmpInfoHeader[6] = (uint8_t)(subWidth >> 16);
    bmpInfoHeader[7] = (uint8_t)(subWidth >> 24);
    bmpInfoHeader[8] = (uint8_t)(subHeight);
    bmpInfoHeader[9] = (uint8_t)(subHeight >> 8);
    bmpInfoHeader[10] = (uint8_t)(subHeight >> 16);
    bmpInfoHeader[11] = (uint8_t)(subHeight >> 24);
    bmpInfoHeader[20] = (uint8_t)(subHeight * rowSize);

    fwrite(bmpHeader, 1, 14, outputImage);
    fwrite(bmpInfoHeader, 1, 40, outputImage);
    for (int i = y; i < y + subHeight; i++)
    {
        fwrite(imageData + (i * width + x) * 3, 1, subWidth * 3, outputImage);
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
    uint8_t bmpHeader[14] = {0x42, 0x4D, 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    uint8_t bmpInfoHeader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int rowSize = ((width * 3 + 3) / 4) * 4;
    bmpHeader[2] = (uint8_t)(54 + height * rowSize);
    bmpInfoHeader[4] = (uint8_t)(width);
    bmpInfoHeader[5] = (uint8_t)(width >> 8);
    bmpInfoHeader[6] = (uint8_t)(width >> 16);
    bmpInfoHeader[7] = (uint8_t)(width >> 24);
    bmpInfoHeader[8] = (uint8_t)(height);
    bmpInfoHeader[9] = (uint8_t)(height >> 8);
    bmpInfoHeader[10] = (uint8_t)(height >> 16);
    bmpInfoHeader[11] = (uint8_t)(height >> 24);
    bmpInfoHeader[20] = (uint8_t)(height * rowSize);

    fwrite(bmpHeader, 1, 14, outputImage);
    fwrite(bmpInfoHeader, 1, 40, outputImage);

    for (int i = height - 1; i >= 0; i--)
    {
        fwrite(imageData + i * width * 3, 1, width * 3, outputImage);
        for (int j = 0; j < (4 - (width * 3) % 4) % 4; j++)
        {
            fputc(0, outputImage);
        }
    }

    fclose(outputImage);
}

void *reconstructImage(int num_rows, int num_cols, int small_width, int small_height)
{
    int width = num_cols * small_width;
    int height = num_rows * small_height;
    int imageSize = width * height * 3;
    uint8_t *reconstructedImageData = (uint8_t *)malloc(imageSize);

    if (reconstructedImageData == NULL)
    {
        printf("Memory allocation error for reconstructed image.\n");
        return NULL;
    }
    for (int i = 0; i < imageSize; i++)
    {
        reconstructedImageData[i] = 0;
    }
    for (int j = num_cols - 1; j >= 0; j--)
    {
        for (int i = num_rows - 1; i >= 0; i--)
        {
            int x = i * small_width;
            int y = j * small_height;
            char filename[100];
            snprintf(filename, sizeof(filename), "output_%d_%d.bmp", j, i);
            FILE *brokenImage = fopen(filename, "rb");
            if (brokenImage == NULL)
            {
                printf("Error opening %s for reading.\n", filename);
                free(reconstructedImageData);
                return NULL;
            }
            fseek(brokenImage, 54, SEEK_SET);
            for (int h = 0; h < small_height; h++)
            {
                fread(reconstructedImageData + ((y + h) * width + x) * 3, 1, small_width * 3, brokenImage);
                for (int pad = 0; pad < (4 - (small_width * 3) % 4) % 4; pad++)
                {
                    fgetc(brokenImage);
                }
            }
            fclose(brokenImage);
        }
    }

    if (reconstructedImageData != NULL)
    {
        const char *reconstructedFilename = "reconstructed_image.bmp";
        saveImageAsBMP(reconstructedImageData, num_cols * small_width, num_rows * small_height, reconstructedFilename);
        printf("Reconstructed image saved as %s\n", reconstructedFilename);
        free(reconstructedImageData);
    }
}
int main()
{
    MPI_Init(NULL, NULL);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    FILE *inputImage = fopen("/home/dmacs/Desktop/MTech/103P/image.bmp", "rb");
    if (inputImage == NULL)
    {
        printf("Error opening the input image.\n");
        return 1;
    }
    uint8_t bmpHeader[54];
    if (fread(bmpHeader, 1, 54, inputImage) != 54 || bmpHeader[0] != 'B' || bmpHeader[1] != 'M')
    {
        printf("Invalid BMP file.\n");
        fclose(inputImage);
        return 1;
    }

    int width = *((int *)&bmpHeader[18]);
    int height = *((int *)&bmpHeader[22]);
    int imageSize = width * height * 3;
    uint8_t *imageData = (uint8_t *)malloc(imageSize);
    if (imageData == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(inputImage);
        return 1;
    }

    if (fread(imageData, 1, imageSize, inputImage) != imageSize)
    {
        printf("Error reading image data.\n");
        fclose(inputImage);
        free(imageData);
        return 1;
    }

    int num_rows = 2;
    int num_cols = 2;
    int small_height = height / num_rows;
    int small_width = width / num_cols;

    if (world_rank == 0)
    {
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                int x = j * small_width;
                int y = i * small_height;

                char filename[100];
                snprintf(filename, sizeof(filename), "output_%d_%d.bmp", i, j);
                saveSubImage(imageData, width, height, x, y, small_width, small_height, filename);
                printf("Saved %s\n", filename);
            }
        }
    }

    // send these images to different processes

    // receive them back

    // Reconstruct the original image
    reconstructImage(num_rows, num_cols, small_width, small_height);
    
    free(imageData);
    fclose(inputImage);
    return 0;
}