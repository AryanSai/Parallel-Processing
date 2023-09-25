#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define the chunk size (adjust as needed)
#define CHUNK_WIDTH 100
#define CHUNK_HEIGHT 100

// Function to read and break the image into smaller chunks
void breakImage(const char* inputFileName) {
    FILE* inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        perror("Error opening input file");
        return;
    }

    // Determine the image width and height
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    rewind(inputFile);

    uint8_t* imageData = (uint8_t*)malloc(fileSize);
    if (!imageData) {
        perror("Memory allocation error");
        fclose(inputFile);
        return;
    }

    fread(imageData, 1, fileSize, inputFile);
    fclose(inputFile);

    // Calculate the number of chunks and dimensions
    int imageWidth = 2;
    int imageHeight = 2;
    int numChunksX = (imageWidth + CHUNK_WIDTH - 1) / CHUNK_WIDTH;
    int numChunksY = (imageHeight + CHUNK_HEIGHT - 1) / CHUNK_HEIGHT;

    // Loop to break the image into smaller chunks
    int chunkNumber = 0;
    for (int y = 0; y < numChunksY; y++) {
        for (int x = 0; x < numChunksX; x++) {
            int chunkStartX = x * CHUNK_WIDTH;
            int chunkStartY = y * CHUNK_HEIGHT;
            int chunkEndX = (x + 1) * CHUNK_WIDTH;
            int chunkEndY = (y + 1) * CHUNK_HEIGHT;

            // Ensure the chunk dimensions do not exceed the image dimensions
            if (chunkEndX > imageWidth) {
                chunkEndX = imageWidth;
            }
            if (chunkEndY > imageHeight) {
                chunkEndY = imageHeight;
            }

            // Create and save the chunk as a separate file
            char outputFileName[100];
            sprintf(outputFileName, "chunk_%d.jpg", chunkNumber);
            FILE* outputFile = fopen(outputFileName, "wb");
            if (!outputFile) {
                perror("Error creating output file");
                free(imageData);
                return;
            }

            // Write the chunk data to the output file
            for (int j = chunkStartY; j < chunkEndY; j++) {
                for (int i = chunkStartX; i < chunkEndX; i++) {
                    int pixelIndex = (j * imageWidth + i) * 3;
                    fwrite(&imageData[pixelIndex], 1, 3, outputFile);
                }
            }

            fclose(outputFile);
            chunkNumber++;
        }
    }

    free(imageData);
}

int main() {
    const char* inputFileName = "/home/dmacs/Desktop/MTech/103P/image.jpg";
    breakImage(inputFileName);
    return 0;
}
