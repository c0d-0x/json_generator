# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Wextra -Wformat-security -pedantic
MAIN = main.c

# Source and object files
CFILES = ./src/*.c
OBJFILES = *.o  

# Target binaries
BIN = test



# Main target
all: $(BIN)
	@echo "Build Complete..."


# Build executable
$(BIN): $(OBJFILES)
	$(CC) $(CFLAGS) $(MAIN) -o $@ $^

# Compile source files
$(OBJFILES): $(CFILES)
	$(CC) $(CFLAGS) -c $^ 


# Phony target (no actual command)
.PHONY: clean uninstall all test

# Clean target
clean:
	rm *.o $(BIN)


