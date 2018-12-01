#include "a4.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void mutate(Individual *individual, double rate)
{

    int n = (individual->image.height) * (individual->image.width);

    int num_pixels = (int)(rate / 100 * n);

    for (int i = 0; i < num_pixels; i++)
    {
        int r_pix = rand() % n;

        individual->image.data[r_pix].r = rand() % 255;
        individual->image.data[r_pix].g = rand() % 255;
        individual->image.data[r_pix].b = rand() % 255;
    }
}

void mutate_population(Individual *individual, int population_size, double rate)
{
    for (int i = (population_size / 4); i < population_size; i++)
    {
        mutate(&individual[i], rate);
    }
}
