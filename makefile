# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Wextra -Wformat-security -pedantic

# Source and object files
CFILE = example.c
# Target binaries
BIN = test

# Main target
all: 
	$(CC) $(CFLAGS) -o $(BIN) $(CFILE)
	@echo "Build Complete..."

# Clean target
clean:
	rm $(BIN)


