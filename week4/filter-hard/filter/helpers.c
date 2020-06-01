#include "helpers.h"
#include <math.h>
#include <stdbool.h>

RGBTRIPLE average_color(RGBTRIPLE neighbor_pixels[], int n_pixels);
RGBTRIPLE sobel_filter(RGBTRIPLE neighbor_pixels[]);
int ceiling(int result);

int GX[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
int GY[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp; // temporary variable to switch right & left pixel

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp.rgbtBlue = image[i][j].rgbtBlue;
            temp.rgbtRed = image[i][j].rgbtRed;
            temp.rgbtGreen = image[i][j].rgbtGreen;

            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;

            image[i][width - j - 1].rgbtBlue = temp.rgbtBlue;
            image[i][width - j - 1].rgbtGreen = temp.rgbtGreen;
            image[i][width - j - 1].rgbtRed = temp.rgbtRed;
        }
    }
}

// Blur image using box-blur
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE neighbor_pixels[9]; // temporary array containing neighbor pixels
    RGBTRIPLE new_image[height][width];
    int n;

    // Explicitely setting the neighbor pixels is still doable in 2D, but is getting
    // a little verbose. A loop would + condition would also work
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            neighbor_pixels[0] = image[i][j];

            if (i == 0) // top
            {
                n = 4;

                if (j == 0) // top-left corner

                {
                    neighbor_pixels[1] = image[i + 1][j + 1];
                    neighbor_pixels[2] = image[i][j + 1];
                    neighbor_pixels[3] = image[i + 1][j];
                }
                else if (j == width - 1) // top-right corner
                {
                    neighbor_pixels[1] = image[i + 1][j];
                    neighbor_pixels[2] = image[i + 1][j - 1];
                    neighbor_pixels[3] = image[i][j - 1];
                }
                else
                {
                    n = 6;
                    neighbor_pixels[1] = image[i][j - 1];
                    neighbor_pixels[2] = image[i + 1][j - 1];
                    neighbor_pixels[3] = image[i + 1][j];
                    neighbor_pixels[4] = image[i + 1][j + 1];
                    neighbor_pixels[5] = image[i][j + 1];
                }
            }
            else if (i == height - 1) // bottom
            {
                n = 4;

                if (j == 0) // bottom-left corner
                {
                    neighbor_pixels[1] = image[i - 1][j];
                    neighbor_pixels[2] = image[i - 1][j + 1];
                    neighbor_pixels[3] = image[i][j + 1];
                }
                else if (j == width - 1) // bottom-right corner
                {
                    neighbor_pixels[1] = image[i][j - 1];
                    neighbor_pixels[2] = image[i - 1][j - 1];
                    neighbor_pixels[3] = image[i - 1][j];
                }
                else
                {
                    n = 6;
                    neighbor_pixels[1] = image[i][j - 1];
                    neighbor_pixels[2] = image[i - 1][j - 1];
                    neighbor_pixels[3] = image[i - 1][j];
                    neighbor_pixels[4] = image[i - 1][j + 1];
                    neighbor_pixels[5] = image[i][j + 1];
                }
            }
            else if (j == 0) // left
            {
                n = 6;
                neighbor_pixels[1] = image[i - 1][j];
                neighbor_pixels[2] = image[i - 1][j + 1];
                neighbor_pixels[3] = image[i][j + 1];
                neighbor_pixels[4] = image[i + 1][j + 1];
                neighbor_pixels[5] = image[i + 1][j];
            }
            else if (j == width - 1) // right
            {
                n = 6;
                neighbor_pixels[1] = image[i - 1][j - 1];
                neighbor_pixels[2] = image[i - 1][j];
                neighbor_pixels[3] = image[i + 1][j];
                neighbor_pixels[4] = image[i + 1][j - 1];
                neighbor_pixels[5] = image[i][j - 1];
            }
            else // center pixels
            {
                n = 9;
                neighbor_pixels[1] = image[i - 1][j - 1];
                neighbor_pixels[2] = image[i - 1][j];
                neighbor_pixels[3] = image[i - 1][j + 1];
                neighbor_pixels[4] = image[i][j + 1];
                neighbor_pixels[5] = image[i + 1][j + 1];
                neighbor_pixels[6] = image[i + 1][j];
                neighbor_pixels[7] = image[i + 1][j - 1];
                neighbor_pixels[8] = image[i][j - 1];
            }

            // passing the number of pixels is not optimal
            // using the size of the array would be better
            new_image[i][j] = average_color(neighbor_pixels, n);
        }
    }

    // saving the new image would be more efficient (and possible), but the exercise
    // stipulates that the function should mutate the image parameter.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
}
// Detect edges using Sobel edge detection algorithm
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE neighbor_pixels[9]; // temporary array containing neighbor pixels
    RGBTRIPLE new_image[height][width];
    RGBTRIPLE pixel;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // iterate over neighbor pixels. k = rows, j = columns
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (k + i < 0 || k + i > height - 1) // overflow left and right
                    {
                        pixel = (RGBTRIPLE)
                        {
                            .rgbtRed = 0, .rgbtGreen = 0, .rgbtBlue = 0
                        };
                    }
                    else if (j + l < 0 || j + l > width - 1) // overflow top and bottom
                    {
                        pixel = (RGBTRIPLE)
                        {
                            .rgbtRed = 0, .rgbtGreen = 0, .rgbtBlue = 0
                        };
                    }
                    else
                    {
                        pixel = image[i + k][j + l];
                    }

                    // Formula to arrange the pixels in this grid:
                    // 0 1 2
                    // 3 4 5
                    // 6 7 8
                    neighbor_pixels[(k + 1) + (l + 1) * 3] = pixel;
                }
            }

            new_image[i][j] = sobel_filter(neighbor_pixels);
        }
    }

    // saving the new image would be more efficient (and possible), but the exercise
    // stipulates that the function should mutate the image parameter.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
}

RGBTRIPLE sobel_filter(RGBTRIPLE neighbor_pixels[])
{
    float sum_x_red = 0.0;
    float sum_x_green = 0.0;
    float sum_x_blue = 0.0;
    float sum_y_red = 0.0;
    float sum_y_green = 0.0;
    float sum_y_blue = 0.0;

    for (int i = 0; i < 9; i++)
    {
        sum_x_red += neighbor_pixels[i].rgbtRed * GX[i];
        sum_x_green += neighbor_pixels[i].rgbtGreen * GX[i];
        sum_x_blue += neighbor_pixels[i].rgbtBlue * GX[i];

        sum_y_red += neighbor_pixels[i].rgbtRed * GY[i];
        sum_y_green += neighbor_pixels[i].rgbtGreen * GY[i];
        sum_y_blue += neighbor_pixels[i].rgbtBlue * GY[i];
    }

    int red = ceiling((sqrt(sum_x_red * sum_x_red + sum_y_red * sum_y_red)));
    int green = ceiling((sqrt(sum_x_green * sum_x_green + sum_y_green * sum_y_green)));
    int blue = ceiling((sqrt(sum_x_blue * sum_x_blue + sum_y_blue * sum_y_blue)));

    return (RGBTRIPLE)
    {
        .rgbtRed = red, .rgbtGreen = green, .rgbtBlue = blue
    };
}

RGBTRIPLE average_color(RGBTRIPLE neighbor_pixels[], int n_pixels)
{
    int red = 0;
    int green = 0;
    int blue = 0;

    for (int i = 0; i < n_pixels; i++)
    {
        red += neighbor_pixels[i].rgbtRed;
        green += neighbor_pixels[i].rgbtGreen;
        blue += neighbor_pixels[i].rgbtBlue;
    }

    return (RGBTRIPLE)
    {
        .rgbtRed = round(red / (float)n_pixels), .rgbtGreen = round(green / (float)n_pixels), .rgbtBlue = round(blue / (float)n_pixels)
    };
}

int ceiling(int result)
{
    if (result > 255)
    {
        return 255;
    }

    return result;
}