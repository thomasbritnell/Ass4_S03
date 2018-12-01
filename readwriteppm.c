#include "a4.h"
#include <stdio.h>
#include <stdlib.h>

PPM_IMAGE *read_ppm(const char *file_name)
{ //takes a file and makes an object with it

    FILE *f = fopen(file_name, "r");
    PPM_IMAGE *r_image = malloc(sizeof(PPM_IMAGE));

    char height[4]; //these are pointers to 4 bit char arrays
    char width[4];
    char max_rgb[4];

    fscanf(f, "%s", width); //skips over the first element
    fscanf(f, "%s", width);
    fscanf(f, "%s", height);
    fscanf(f, "%s", max_rgb);
    int a_size = atoi(height) * atoi(width);

    PIXEL *data = malloc(sizeof(PIXEL) * a_size);
    char r[4];
    char g[4];
    char b[4];

    for (int i = 0; i < a_size; i++)
    { //initialize the data array

        fscanf(f, "%s %s %s", r, g, b);

        data[i].r = atoi(r);
        data[i].g = atoi(g);
        data[i].b = atoi(b);
    }
    fclose(f);

    r_image->height = atoi(height);
    r_image->width = atoi(width);
    r_image->max_color = atoi(max_rgb);
    r_image->data = data; //pointer to first element of data array

    return r_image;
}

void write_ppm(const char *file_name, const PPM_IMAGE *image)
{
    FILE *f = fopen(file_name, "w");
    fprintf(f, "%s\n%d %d\n%d\n", "P3", (image->width), (image->height), image->max_color);

    for (int i = 0; i < (image->height * image->width); i++)
    {

        fprintf(f, "%d %d %d ", image->data[i].r, image->data[i].g, image->data[i].b);
    }

    fclose(f);
}
