# Benchmarking and Multithreading project

## benchmark.c

A buffer proping mechanism that reports values in .csv format with increasing buffer sizes. Simply
run with `make all` and `./benchmark` to report a list of read times for buffers of increasing size.

## tests.cc

A testing platfrom for the barrier types implemented in `barreirs.cc` and defined in `barriers.hh`.
Run with `make all` and `./tests <flag>` where `<flag>` can be any one of `-t`, `-c`, `-d`, or `-m`
to test the timing of barriers or run suite tests on any specified type individuallay.

