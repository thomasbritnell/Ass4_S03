#include "a4.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int comparator(const void *p, const void *q)
{
    typedef struct
    {
        PPM_IMAGE image; // image
        double fitness;  // fitness
    } Individual;

    int l = ((Individual *)p)->fitness;
    int r = ((Individual *)q)->fitness;
    return (l - r);
}

PPM_IMAGE *evolve_image(const PPM_IMAGE *image, int num_generations, int population_size, double rate)
{
    //image is the goal image
    Individual *pop = generate_population(population_size, image->width, image->height, image->max_color); //need to free pop
    comp_fitness_population(image->data, pop, population_size);                                            //gives each individual a fitness value
    qsort(pop, population_size, sizeof(Individual), comparator);

    for (int i = 0; i < num_generations; i++)
    {
        crossover(pop, population_size);
        mutate_population(pop, population_size, rate);
        comp_fitness_population(image->data, pop, population_size);
        qsort(pop, population_size, sizeof(Individual), comparator);
    }
    write_ppm("temp.ppm", &(pop->image));
    PPM_IMAGE *final = read_ppm("temp.ppm");
    //kept getting segmentation fault, so transfers the content into a temporary file, which is removed by the free_image function

    for (int i = population_size - 1; i >= 0; i--)
    {
        free((pop + i)->image.data);
    }
    free(pop);

    return final;
}

void free_image(PPM_IMAGE *p)
{
    free(p->data);
    free(p);
    remove("temp.ppm");
}