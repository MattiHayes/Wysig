CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

wysig: ./src/main.c
	$(CC) -o ./bin/main ./src/main.c $(CFLAGS)

run:
	./bin/main