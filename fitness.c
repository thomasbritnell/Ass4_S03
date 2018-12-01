#include "a4.h"
#include <stdio.h>
#include <stdlib.h>

double comp_distance(const PIXEL *A, const PIXEL *B, int image_size)
{

    double total = 0;
    for (int i = 0; i < image_size; i++)
    {
        double Red = A[i].r - B[i].r;
        double Green = A[i].g - B[i].g;
        double Blue = A[i].b - B[i].b;
        total += (Red * Red + Green * Green + Blue * Blue);
    }

    return sqrt(total);
}

void comp_fitness_population(const PIXEL *image, Individual *individual, int population_size)
{
    for (int i = 0; i < population_size; i++)
    {
        (individual + i)->fitness = comp_distance((individual + i)->image.data, image, individual->image.height * individual->image.width);
    }
}