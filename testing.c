#include "a4.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

PPM_IMAGE *read_ppm(const char *file_name){//takes a file and makes an object with it

    FILE *f = fopen( file_name, "r" );
    PPM_IMAGE *r_image = malloc(sizeof(PPM_IMAGE));

    char height[4];//these are pointers to 4 bit char arrays
    char width[4];
    char max_rgb[4]; 

    fscanf(f,"%s",width);//skips over the first element
    fscanf(f,"%s", width);
    fscanf(f,"%s", height);
    fscanf(f,"%s",max_rgb);
    int a_size = atoi(height)*atoi(width);
    
    //PIXEL data[a_size]; //the 1-d array that will hold the pixel data
    PIXEL *data = malloc(sizeof(PIXEL)*a_size);
    char r[4];
    char g[4];
    char b[4];
    
    for(int i = 0;i<a_size;i++){//initialize the data array
        // for(int j =0;j<=atoi(width);j++){
            fscanf(f,"%s %s %s", r,g,b);
            // fscanf(f,"%s", g);
            // fscanf(f,"%s", b);
            //PIXEL temp; 
            data[i].r = atoi(r);
            data[i].g = atoi(g);
            data[i].b = atoi(b);
           // data[i] = temp;
           // }
    }
    fclose(f);

    r_image->height = atoi(height);
    r_image->width= atoi(width);
    r_image->max_color = atoi(max_rgb);
    r_image->data = data;//pointer to first element of data array
    
    return r_image;

}

void write_ppm(const char *file_name, const PPM_IMAGE *image){
    FILE *f = fopen( file_name, "w" );
    fprintf(f,"%s\n%d %d\n%d\n","P3",(image->width),(image->height),image->max_color);

   for(int i = 0;i<(image->height*image->width);i++){
            
     fprintf(f,"%d %d %d ",image->data[i].r,image->data[i].g,image->data[i].b);
        
     }

    fclose(f);
}

PIXEL *generate_random_image(int width, int height, int max_colour){

    PIXEL *img= malloc(sizeof(PIXEL)*(width*height));//1d array of pixels
    
    for (int i=0;i<(width*height);i++){
        img[i].r = rand()%(max_colour+1); 
        img[i].g = rand()%(max_colour+1); 
        img[i].b = rand()%(max_colour+1);
    }
    return img;
}

Individual *generate_population(int population_size, int width, int height, int max_color){
    Individual *pop = malloc(sizeof(Individual)*population_size);//an array of individuals

    for(int i=0;i<population_size;i++){
        (pop+i)->image.height = height;
        (pop+i)->image.width = width;
        (pop+i)->image.max_color = max_color;
        (pop+i)->image.data = generate_random_image(height,width,max_color);
    }
    return pop;

}

double comp_distance(const PIXEL *A, const PIXEL *B, int image_size){
    // A and B are pointers to an array of pixel objects (therefore, each entry in the array is an object, not a pointer)
    double total = 0;
    for(int i=0;i<image_size;i++){
        double Red = A[i].r - B[i].r ;
        double Green = A[i].g - B[i].g ;
        double Blue = A[i].b - B[i].b ;
        total += (Red*Red + Green*Green + Blue*Blue);    
    }
    
    return sqrt(total);
    
    
}

void comp_fitness_population(const PIXEL *image, Individual *individual, int population_size){
    for(int i= 0; i<population_size;i++){
    (individual+i)->fitness = comp_distance((individual+i)->image.data,image,individual->image.height*individual->image.width);
    
    }

}

void recombine(Individual *parent1, Individual *parent2, Individual *child1, Individual *child2) {
  // Generate a random number between 0 and image size-1
  int image_size = parent1->image.width * parent1->image.height;
  int crossover = rand() % image_size;
  int i;

  // Up to crossover child1 "inherits" from parent1 and child2 inherits
  // from parent2
  for (i = 0; i < crossover; i++) {
    child1->image.data[i] = parent1->image.data[i];
    child2->image.data[i] = parent2->image.data[i];
  }
  // After that child1 "inherits" from parent2 and child2 inherits
  // from parent1
  for (i = crossover; i < image_size; i++) {
    child1->image.data[i] = parent2->image.data[i];
    child2->image.data[i] = parent1->image.data[i];
  }
}

void crossover(Individual *population, int population_size) {
  
  for (int i = 0; i < population_size / 2; i += 2)
    // parent i and i+1 produce children population_size/2+i and
    // population_size/2+i+1.
    recombine(population + i, population + i + 1,
              population + population_size / 2 + i,
              population + population_size / 2 + i + 1);
}

void mutate(Individual *individual, double rate){
    
     
    //int n = 1;

   // if ((individual->image.height)*(individual->image.width)!=0){
       int n = (individual->image.height)*(individual->image.width);
    //}
    
   // printf("%d\n",n);

   
    int num_pixels = (int)(rate/100*n);
   
    for (int i= 0;i<num_pixels;i++){
        int r_pix = rand() % n;
      
        individual->image.data[r_pix].r = rand()%255;
        individual->image.data[r_pix].g = rand()%255;
        individual->image.data[r_pix].b = rand()%255;
    }

}

void mutate_population(Individual *individual, int population_size, double rate){
    for (int i=(population_size/4);i<population_size;i++){
        mutate(&individual[i],rate);
    }
}

int comparator(const void *p, const void *q)  { 
        typedef struct {
        PPM_IMAGE image; // image
        double fitness;  // fitness
        } Individual;

    int l = ((Individual *)p)->fitness; 
    int r = ((Individual *)q)->fitness;  
    return (l - r); 
} 


PPM_IMAGE *evolve_image(const PPM_IMAGE *image, int num_generations, int population_size, double rate){
    //image is the goal image
    Individual *pop = generate_population(population_size,image->width,image->height,image->max_color);//need to free pop
    comp_fitness_population(image->data,pop,population_size);//gives each individual a fitness value
    qsort(pop,population_size,sizeof(Individual),comparator);
    
    for(int i=0;i<num_generations;i++){
        crossover(pop,population_size);
        mutate_population(pop,population_size,rate);
        comp_fitness_population (image->data,pop,population_size);
        qsort(pop,population_size,sizeof(Individual),comparator);


        //printf("%.2f\n",pop->fitness);
    }

    write_ppm("temp.ppm",&(pop->image));
   PPM_IMAGE *final = read_ppm("temp.ppm");
    
    //*final = (pop->image);
    //final->data = pop->image.data;


    for (int i=population_size-1;i>=0;i--){
        free((pop+i)->image.data);
    }
    free(pop);
    


     return final;
}

// static void printPixel(PIXEL p){
   
//     printf("%d %d %d\n", p.r , p.g , p.b);
  
// }
void free_image(PPM_IMAGE *p){
    free(p->data);
    free(p);
    remove("temp.ppm");
}

int main(){

    int num_generations = 40;
    int population_size = 24;
    double mutation_rate = 3;

      PPM_IMAGE *goal = read_ppm("me.ppm");

    // PIXEL *pixelmap = generate_random_image(34,13,89);
    // Individual *pop = generate_population(population_size,45,62,25);
    // comp_fitness_population(goal->data,pop,population_size);
    // qsort(pop,population_size,sizeof(Individual),comparator);

    // //freeing requirements

    // //for each PPM_IMAGE from read_ppm:
    // free(goal->data);
    // free(goal);

    // //for each pixel array from genrandomimg
    // free(pixelmap);

    // //for each population array
    // for (int i = population_size-1;i>=0;i--){
    //    free((pop+i)->image.data);
    // }
    // free(pop);



    

  printf("\nFile %s:\n %dx%d, max color %d, pixels to mutate %d\n",
	 "me.ppm", goal->width, goal->height, goal->max_color, (int)(mutation_rate/100*goal->width*goal->height));

  // Compute image
  PPM_IMAGE *new_image = evolve_image(goal, num_generations, population_size, mutation_rate);

  // Write image
  write_ppm("rip_pepe.ppm", new_image);
  // Free memory
  free_image(goal);
  free_image(new_image);
   
    return 0;
}