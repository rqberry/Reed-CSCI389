#include <stdio.h>
#include "rand_list.h"

/* main takes input from the command line in the argv parameter, ensures
   the correct number of parameters was given, and then runs gen_rand_list
   and print_list_mean accordingly.
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
  int32_t *rand_list = gen_rand_list(list_size, list_seed);
  print_list_mean(rand_list, list_size);
  // free the allocated space of gen_rand_list
  free(rand_list);
  return 0;
}
