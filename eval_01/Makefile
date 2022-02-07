CC = gcc
CCFLAGS = -Wall -Werror -Wextra -pedantic


all: update_locations

update_locations: update_locations.c
	$(CC) -o update_locations update_locations.c $(CCFLAGS)

clean:
	rm update_locations
