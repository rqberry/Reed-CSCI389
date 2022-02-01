CC = gcc
CCFLAGS = -Wall -Werror -Wextra -pedantic


all: rand_list

rand_list: rand_list.c
	$(CC) -o rand_list_exe rand_list.c $(CCFLAGS)

clean:
	rm rand_list_exe
