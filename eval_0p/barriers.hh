#include <stdint.h> // for int32_t
#include <atomic>   // for std::atomic
#include <mutex>    // for thread_local
#include <math.h>   // for log2

#pragma once

void centralized_barrier(int32_t tid, const int32_t P);

void dissemination_barrier(int32_t tid, int32_t P);

void mcs_barrier(int32_t tid, int32_t P);
