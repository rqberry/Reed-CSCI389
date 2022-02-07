#include <stdlib.h>
#include <stdio.h>
#include <stdint.h> // for int_t types

typedef int64_t pos_t;

// functions
pos_t *generate_random_list(int32_t size, int32_t bound);
void update_coords(pos_t *xs, pos_t *ys, pos_t *zs, pos_t *vx, pos_t *vy, pos_t *vz, int32_t size);
pos_t sum(pos_t *list, int32_t size);
