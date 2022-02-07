#include <time.h>  // for time()
#include "update_locations.h"

// used for benchmarking large amounts of data
const int8_t BNCHMRK = 0;

/* generate_random_list: Create a list of 'size' pos_t numbers in the 
   range [-bound, bound]
 */
pos_t *generate_random_list(int32_t size, int32_t bound) {
  pos_t *random_list = malloc(sizeof(pos_t)*size);
  for (int32_t i = 0; i < size; i++) {
    // for variant pos_t floating point types
    /*
    float rand_loc = (float)rand()/((float)RAND_MAX / (2.0*(float)bound));
    random_list[i] = rand_loc - (float)bound;
    */
    random_list[i] = (((pos_t)rand() % (pos_t)2*(pos_t)bound) - (pos_t)bound);
  }
  return random_list;
}

/* Update positions by velocity, one time-step
 */
void update_coords(pos_t *xs, pos_t *ys, pos_t *zs, pos_t *vx, pos_t *vy, pos_t *vz, int32_t size) {
  for (int32_t i = 0; i < size; i++) {
    xs[i] += vx[i];
    ys[i] += vy[i];
    zs[i] += vz[i];
  }  
}

/* Sum a list of size pos_t numbers
 */
pos_t sum(pos_t *list, int32_t size) {
  pos_t list_sum = 0; 
  for (int32_t i = 0; i  < size; i++) {
    list_sum += list[i];
  }
  return list_sum;
}

int main(int argc, char **argv) {
  // assert the correct number of arguments is passed
  if (argc != 3) {
    printf("Required arguments: vector_length(N) and iterations_num(M)\n");
    return 1;
  }
  // get size and seed parameters from argv
  int32_t size = atoi(argv[1]);
  // assert the list size is valid
  if (size == 0) {
    printf("Invalid list size.\n");
    return 1;
  }
  int32_t iters = atoi(argv[2]);
  // assert the list seed is valid
  if (iters == 0) {
    printf("Invalid iteration number.\n");
    return 1;
  }
  srand(size);
  // CHECK  do bounds need to be floats?
  // initialize positions
  pos_t *xs = generate_random_list(size, 1000);
  pos_t *ys = generate_random_list(size, 1000);
  pos_t *zs = generate_random_list(size, 1000);
  pos_t *vx = generate_random_list(size, 1);
  pos_t *vy = generate_random_list(size, 1);
  pos_t *vz = generate_random_list(size, 1);
  // run update_coords iters number of times
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int32_t i = 0; i < iters; i++) {
    update_coords(xs, ys, zs, vx, vy, vz, size);
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  
  // compute checksum
  pos_t chksum = sum(xs, size) + sum(ys, size) + sum(zs, size);
  // compute the start and end times in microseconds
  double start_time = ((double)start.tv_sec * 1e6) + ((double)start.tv_nsec * 1e-3);
  double end_time = ((double)(end.tv_sec * 1e6)) + ((double)end.tv_nsec * 1e-3);
  // compute the elapsed and mean times in microseconds
  double elapsed_time = (end_time - start_time);
  double mean_time = elapsed_time / (double)(size * iters);
  // print results limited print for benchmarking
  if (BNCHMRK) {
    printf("%.15f\n", mean_time);
  } else {
    printf("Mean time per coordinate: %fus\n", mean_time);
    printf("Final checksum is: %li\n", chksum);
  }
  // free all allocated space before exiting
  free(xs);
  free(ys);
  free(zs);
  free(vx);
  free(vy);
  free(vz);
  return 0;
}


