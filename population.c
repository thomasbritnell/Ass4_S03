#include "a4.h"
#include <stdio.h>
#include <stdlib.h>

PIXEL *generate_random_image(int width, int height, int max_colour)
{

    PIXEL *img = malloc(sizeof(PIXEL) * (width * height)); //1d array of pixels

    for (int i = 0; i < (width * height); i++)
    {
        img[i].r = rand() % (max_colour + 1);
        img[i].g = rand() % (max_colour + 1);
        img[i].b = rand() % (max_colour + 1);
    }
    return img;
}

Individual *generate_population(int population_size, int width, int height, int max_color)
{
    Individual *pop = malloc(sizeof(Individual) * population_size); //an array of individuals

    for (int i = 0; i < population_size; i++)
    {
        (pop + i)->image.height = height;
        (pop + i)->image.width = width;
        (pop + i)->image.max_color = max_color;
        (pop + i)->image.data = generate_random_image(height, width, max_color);
    }
    return pop;
}