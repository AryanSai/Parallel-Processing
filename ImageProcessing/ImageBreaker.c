#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> //working with integer types

// This function is responsible for saving a portion of the original image as a new BMP file.

// It takes the following parameters:
// imageData: A pointer to the image data (pixel values) of the original image.
// width and height: The width and height of the original image.
// x and y: The coordinates of the top-left corner of the portion to be saved.
// subWidth and subHeight: The width and height of the portion to be saved.
// filename: The name of the BMP file to be created.

// Inside the function:
// open a new BMP file for writing.
// constructs the BMP file headers (bmpHeader and bmpInfoHeader) based on the BMP file format specifications.
// calculates the padding needed for each row to ensure the total row size is a multiple of 4 bytes (required by BMP format).
// updates the header fields with the appropriate values (e.g., file size, image dimensions).
// writes the headers to the new BMP file.
// writes the image data for the specified portion to the file, ensuring proper padding for each row.
// Finally, it closes the output file.
void saveSubImage(uint8_t *imageData, int width, int height, int x, int y, int subWidth, int subHeight, const char *filename)
{
    FILE *outputImage = fopen(filename, "wb");
    if (outputImage == NULL)
    {
        printf("Error opening %s for writing.\n", filename);
        return;
    }

    // BMP header (14 bytes)
    // The first two bytes ('B' and 'M') of the BMP file indicate the file type and serve as a signature.
    // The next four bytes (at offsets 18 to 21) represent the width of the image in pixels.
    // The next four bytes (at offsets 22 to 25) represent the height of the image in pixels.
    // The 54th byte (at offset 53) represents the offset to the image data in the file.

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

uint8_t *reconstructImage(int num_rows, int num_cols, int small_width, int small_height)
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

    return reconstructedImageData;
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

int main()
{
    FILE *inputImage = fopen("/home/dmacs/Desktop/MTech/103P/image.bmp", "rb");
    if (inputImage == NULL)
    {
        printf("Error opening the input image.\n");
        return 1;
    }

    // Read BMP header
    uint8_t bmpHeader[54]; // BMP header is typically 54 bytes in size for standard BMP files
    // verify that the file is a valid BMP file
    if (fread(bmpHeader, 1, 54, inputImage) != 54 || bmpHeader[0] != 'B' || bmpHeader[1] != 'M')
    {
        printf("Invalid BMP file.\n");
        fclose(inputImage);
        return 1;
    }

    // Read image dimensions
    int width = *((int *)&bmpHeader[18]);
    int height = *((int *)&bmpHeader[22]);

    // The size is calculated as width x height x 3 since it's assumed that the image is in 24-bit color format (3 bytes per pixel)
    // Calculate total image size in bytes
    int imageSize = width * height * 3;

    // Read image data
    uint8_t *imageData = (uint8_t *)malloc(imageSize);
    if (imageData == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(inputImage);
        return 1;
    }

    // It reads the image data from the input BMP file into the allocated memory.
    //  If there is an error reading the data, it prints an error message and exits with an error code.
    if (fread(imageData, 1, imageSize, inputImage) != imageSize)
    {
        printf("Error reading image data.\n");
        fclose(inputImage);
        free(imageData);
        return 1;
    }

    int num_rows = 2; // Number of rows for the grid
    int num_cols = 2; // Number of columns for the grid

    int small_height = height / num_rows;
    int small_width = width / num_cols;

    // to iterate through each cell in the grid. For each cell, it calculates
    //  the coordinates of the top-left corner, generates a
    //  filename based on the position, and calls the saveSubImage function to
    //   save the corresponding portion of the original image as a BMP file.
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            // Calculate the coordinates for the top-left corner of the small image
            int x = j * small_width;
            int y = i * small_height;

            // Generate a filename based on the position
            char filename[100];
            snprintf(filename, sizeof(filename), "output_%d_%d.bmp", i, j);

            // Save the small image portion
            saveSubImage(imageData, width, height, x, y, small_width, small_height, filename);

            printf("Saved %s\n", filename);
        }
    }

    free(imageData);

    // send these images to different processes
    // receive them back

    // Reconstruct the original image
    uint8_t *reconstructedImageData = reconstructImage(num_rows, num_cols, small_width, small_height);

    if (reconstructedImageData != NULL)
    {
        // Define the filename for the reconstructed image
        const char *reconstructedFilename = "reconstructed_image.bmp";

        // Save the reconstructed image as a BMP file
        saveImageAsBMP(reconstructedImageData, num_cols * small_width, num_rows * small_height, reconstructedFilename);

        printf("Reconstructed image saved as %s\n", reconstructedFilename);

        // Free the memory allocated for the reconstructed image
        free(reconstructedImageData);
    }

    // After reading the BMP image data, it closes the input file.
    fclose(inputImage);
    return 0;
}