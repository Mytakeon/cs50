#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_jpeg_header(unsigned char *buffer);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid number of arguments. Usage: ./recover image\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file %s.\n", argv[1]);
        return 1;
    }

    int n_images = 0;
    FILE *output_image = NULL;
    char output_image_name[7];
    unsigned char *buffer = malloc(512);

    while (fread(buffer, 512, 1, file))
    {
        if (is_jpeg_header(buffer))
        {
            // Close the previous file if it exists
            if (output_image != NULL)
            {
                fclose(output_image);
            }

            sprintf(output_image_name, "%03i.jpg", n_images);
            output_image = fopen(output_image_name, "w");
            n_images++;
        }

        // Continue writing to image currently openned
        if (output_image != NULL)
        {
            fwrite(buffer, 512, 1, output_image);
        }
    }

    fclose(output_image); // Close the last image written
    fclose(file);
    free(buffer);
    return 0;
}

bool is_jpeg_header(unsigned char *buffer)
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}