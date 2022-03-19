CC = gcc
CPP = g++
CCFLAGS = -Wall -Werror -pedantic


all: tests benchmark

benchmark: benchmark.c
	$(CC) -o benchmark benchmark.c $(CCFLAGS) -O3

tests: tests.cc
	$(CPP) -std=c++17 -g -pthread -o tests tests.cc $(CCFLAGS)
clean:
	rm benchmark
	rm tests
