#include <stdio.h>
#include <stdlib.h>  // for rand() and srand() in gen_rand_list()
#include <stdint.h>  // for int32_t

/* gen_rand_list is a helper function that takes 3 paramaters and writes to
   a list of appropriate size, random 32 bit integers taken from an initial
   32 bit integer seed.

   NOTE: gen_rand_list should only be used within the context of this code 
   as it has no assertions that the provided list is of the appropriate 
   size. This of course is fine given the context of the rand_list function 
   which always pases a list of length "size".
 */
void gen_rand_list(const int32_t size, const int32_t seed, int32_t *rand_list) {
  int32_t list_seed = seed;
  // iterate through the rand_list
  for (int32_t i = 0; i < size; i++) {
    // generate new random 32 bit integer and write it to the list
    srand(list_seed);
    rand_list[i] = rand();    
    list_seed ^= rand_list[i];
  }
}


/* rand_list takes two inputs as 32 bit integers, a size and a seed which
   are then used to make a list of 32 bit integers and passed as parameters
   to the gen_rand_list helper function. The resulting list of random 32 
   bit integers is printed and the mean of the list is calculated and
   printed as well.
 */
void rand_list(const int32_t size, const int32_t seed) {
  // allocate a list of 32 bit integers on the stack
  int32_t rand_list[size];
  // run gen_rand_list helper function
  gen_rand_list(size, seed, rand_list);
  // print the resulting list and the mean of its values
  printf("Random list:");
  double r_sum = 0.0;
  for (int32_t i = 0; i < size; i++) {
    printf(" %i", rand_list[i]);
    r_sum += rand_list[i];
  }
  printf("\nMean: %f\n", r_sum/(double)size);
}

/* main takes input from the command line in the argv parameter, ensures
   the correct number of parameters was given, and then runs rand_list
   accordingly.
 */
int main(int argc, char *argv[]) {
  // assert that the correct number of arguments was give
  if (argc != 3) {
    (argc > 3) ? printf("Too many arguments, saw: ") : printf("Too few arguments, saw: ");
    printf("%d expected: 3.\nExpected arguments: ./<executable> <int list size> <int random seed>\n", argc);
    return 0;
  }
  // get size and seed parameters from argv
  int32_t list_size = atoi(argv[1]);
  // assert the list size is valid
  if (list_size == 0) {
    printf("Invalid list size.\n");
    return 0;
  }
  int32_t list_seed = atoi(argv[2]);
  // assert the list seed is valid
  if (list_seed == 0) {
    printf("Invalid list seed.\n");
    return 0;
  }
  // run the rand_list function which prints the results
  rand_list(list_size, list_seed);
  return 0;
}
