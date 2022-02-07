#include <stdio.h>
#include <stdlib.h>  // for rand() and srand() in gen_rand_list()
#include <stdint.h>  // for int32_t

#pragma once // prevent multiple compilations

/* gen_rand_list: Takes 2 parameters, size and seed, and allocates heap
   storage for a list of 32 bit integers which are randomly generated
   using srand() and rand(). A pointer to the list is then returned.
 */
int32_t *gen_rand_list(const int32_t size, const int32_t seed) {
  int32_t tmp_seed = seed;
  // allocate space for a list of 32 bit integers on the heap
  int32_t *rand_list = malloc(sizeof(int32_t)*size);
  // iterate through the rand_list
  for (int32_t i = 0; i < size; i++) {
    // generate new random 32 bit integer and write it to the list
    srand(tmp_seed);
    rand_list[i] = rand();    
    tmp_seed ^= rand_list[i];
  }
  // return the randomized list
  return rand_list;
}


/* print_list_mean: Takes 2 parameters, a 32 bit int list and its size, 
   and then prints each element of the list keeping a sum of the elements 
   in order to calculate and print the mean of the list's contents.
 */
void print_list_mean(const int32_t *list, const int32_t list_size) {
  // print the resulting list and the mean of its values
  printf("Random list:");
  double sum = 0.0;
  for (int32_t i = 0; i < list_size; i++) {
    printf(" %i", list[i]);
    sum += list[i];
  }
  printf("\nMean: %f\n", sum/(double)list_size);
}