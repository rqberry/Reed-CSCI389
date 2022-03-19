#include <stdint.h> // for int32_t
#include <atomic>   // for std::atomic
#include <mutex>    // for thread_local
#include <math.h>   // for log2

const int32_t P = 32;    // default thread count


/* centralized_barrier
   implementation of a centralized_barrier which utilizes a global
   atomic counter to wait for all P threads to arrive before releasing
   them.
*/
std::atomic<int> counter = 1;   // for centralized_barrier
bool central_sense = false;     // for centralized_barrier

void centralized_barrier(int32_t tid, const int32_t P) {
  bool local_sense =! central_sense;
  // atomic increment
  if (std::atomic_fetch_add(&counter, 1) == P) {
    counter = 1;
    central_sense = local_sense;
  } else {
    while (central_sense != local_sense); // spin
  }
}

/* dissemination_barrier
   utilizes an array of flags with parity control to alert threads
   to when all other threads have reached the barrier.
*/
bool flags[P][2][(int32_t)log2(P)]; // for dissemination_barrier
thread_local bool dis_sense = false;
thread_local int dis_parity = 0;

void dissemination_barrier(int32_t tid, int32_t P) {
  for (int32_t r = 0; r < (int32_t)log2(P); ++r) {
    int32_t partner = (tid + (int32_t)((int32_t)1 << r)) % P;
    flags[partner][dis_parity][r] = !dis_sense;
    while (flags[tid][dis_parity][r] == dis_sense); // spin
  }
  if (dis_parity == 1) {
    dis_sense = !dis_sense;
  }
  dis_parity = 1 - dis_parity;
}



/* mcs_barrier
  implements an mcs_barrier using the array tree structure defined
  below with the following index structure:
    node      - tid := tid
    parent    - pid := (tid-1)/2
    children  - cidl := 2*tid+1
              - cidr := 2*(tid+1)
*/
bool mcs_tree[P][2]; // for mcs_barrier
thread_local bool mcs_sense = true;

void mcs_barrier(int32_t tid, int32_t P) {
  // wait for children
  if (2*tid+2 < P) {  // two children
    // spin until both children are ready
    while (mcs_tree[2*tid+1][0] != mcs_sense &&
           mcs_tree[2*tid+2][0] != mcs_sense) {} // spin
  } else if ((2*tid)+1 < P) { // one child
    // spin until child is ready
    while (mcs_tree[2*tid+1][0] != mcs_sense) {} // spin
  } // done spinning or no children
  // set arrival flag
  mcs_tree[tid][0] = mcs_sense;
  // spin until parent flags release and let thread 0 bypass
  if (tid != 0) {
    if (tid % 2 == 1) {
      while (mcs_tree[(int32_t)((tid-1)/2)][1] != mcs_sense) {} // spin
    } else {
      while (mcs_tree[((int32_t)(tid/2)-1)][1] != mcs_sense) {} // spin
    }
  }
  // wake up children
  mcs_tree[tid][1] = mcs_sense;
  // reverse sense and parity
  mcs_sense = !mcs_sense;
}
/*
void mcs_barrier(int32_t tid, int32_t P, bool *mcs_sense) {
  // wait for children
  if (2*tid+2 < P) {  // two children
    // spin until both children are ready
    while (mcs_tree[2*tid+1][0] != *mcs_sense &&
           mcs_tree[2*tid+2][0] != *mcs_sense) {} // spin
  } else if ((2*tid)+1 < P) { // one child
    // spin until child is ready
    while (mcs_tree[2*tid+1][0] != *mcs_sense) {} // spin
  } // done spinning or no children
  // set arrival flag
  mcs_tree[tid][0] = *mcs_sense;
  // spin until parent flags release and let thread 0 bypass
  if (tid != 0) {
    if (tid % 2 == 1) {
      while (mcs_tree[(int32_t)((tid-1)/2)][1] != *mcs_sense) {} // spin
    } else {
      while (mcs_tree[((int32_t)(tid/2)-1)][1] != *mcs_sense) {} // spin
    }
  }
  // wake up children
  mcs_tree[tid][1] = *mcs_sense;
  // reverse sense and parity
  *mcs_sense = !(*mcs_sense);
}*/
