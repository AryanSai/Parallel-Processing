#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *inputImage = fopen("/home/dmacs/Desktop/MTech/103P/image.bmp", "rb");
    FILE *outputImage = fopen("RotatedImage.bmp", "wb");
    if (inputImage == NULL || outputImage == NULL)
    {
        printf("Error opening files.\n");
        return 1;
    }

    int c;
    while ((c = fgetc(inputImage)) != EOF)
    {
        fputc(c, outputImage);
    }

    printf("Image copied successfully.\n");

    fclose(inputImage);
    fclose(outputImage);

    return 0;
}