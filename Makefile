# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Iinc

# Files
SRC = src/main.c src/terminal.c
OBJ = $(SRC:.c=.o)
BIN = wysig

# Default target
all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: $(BIN)
	./$(BIN)

# Clean compiled files
clean:
	rm -f $(OBJ) $(BIN)
