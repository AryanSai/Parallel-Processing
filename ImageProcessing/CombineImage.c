#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Function to reconstruct the original image from broken images
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
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            // Calculate the coordinates for the top-left corner of the small image
            int x = j * small_width;
            int y = i * small_height;

            // Generate the filename for the broken image
            char filename[100];
            snprintf(filename, sizeof(filename), "output_%d_%d.bmp", i, j);

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

            // Read and copy the pixel data from the broken image to the reconstructed image
            for (int h = 0; h < small_height; h++)
            {
                fseek(brokenImage, (small_height - h - 1) * small_width * 3, SEEK_CUR);
                fread(reconstructedImageData + ((y + h) * width + x) * 3, 1, small_width * 3, brokenImage);
                fseek(brokenImage, (small_height - h - 1) * small_width * 3, SEEK_CUR);
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
    // Define the number of rows, columns, and dimensions of small images
    int num_rows = 2;
    int num_cols = 2;
    int small_width = 320;  // Width of each small image
    int small_height = 240; // Height of each small image

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

    return 0;
}
