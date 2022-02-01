
## rand_list.c

### Description

rand_list.c is a simple C program which takes two command-line arguments in
addition to its' executable file. The program when given a 32 bit integer
size parameter and a 32 bit integer seed paramater generates and prints a
list of random 32 bit integers generated from the provided seed and list
length. The program additionally outputs a double percision floating point
number equivalent to the mean of all the list's entries.

### Compilation and Execution

rand_list.c can be compiled via the included Makefile using the commands
"make all" or "make rand_list" or by the command line command:

      gcc -o <executable> rand_list.c -Wall -Werror -Wextra -pedantic

To execute the produced executable file follow the format below:

   ./<executable> <list_size> <list_seed>