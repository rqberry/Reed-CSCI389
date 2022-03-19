#include <stdlib.h>
#include <stdint.h>     // intX_t
#include <stdio.h>
#include <time.h>

// attempt to beat the prefetcher
double probe_buffer(char *buffer, int64_t nprobes, int64_t buffer_size) {

  int64_t *r_values = malloc(sizeof(int64_t)*8);
  srand(0);
  // precalculate random index values
  for (int32_t i = 0; i < 7; ++i) r_values[i] = rand() % buffer_size;
  // benchmarking
  struct timespec start, end;

  volatile char c = 0;
  int64_t index = 0;

  // loop and calculate
  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int64_t i = 0; i < nprobes; ++i) {
    index = (index+ r_values[index % 7]) % buffer_size;
    c = buffer[index];
  }
  c++;

  clock_gettime(CLOCK_MONOTONIC, &end);
  // compute the start and end times in microseconds
  double start_time = (double)(start.tv_sec * 1e9) + (double)start.tv_nsec;
  double end_time = (double)(end.tv_sec * 1e9) + (double)end.tv_nsec;
  // compute the elapsed and mean times in microseconds
  double elapsed_time = (end_time - start_time);
  return elapsed_time / (double)nprobes;
}

void benchmark_sizes(int64_t nprobes) {
  printf("%s, %s\n", "size_kb", "time");
  for (int64_t i = 4; i < 33; ++i) {
    int64_t buffer_size;
    for (buffer_size = ((int64_t)1 << i);
      buffer_size + buffer_size/4 < ((int64_t)1 << (i+1));
      buffer_size += buffer_size/4) {
        char *buffer = malloc(sizeof(char)*buffer_size);
        printf("%f, %f\n", (double)(buffer_size), probe_buffer(buffer, nprobes, buffer_size));
        free(buffer);
    }
  }

}

int main() {

  benchmark_sizes(1000);

  return 0;
}
