# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Wextra -Wformat-security -pedantic -fPIC
MAIN = example.c

# Source and object files
CFILES = ./src/*.c
OBJFILES = *.o 

# Target binaries
BIN = test
LIBNAME = libjson_gen
STATIC_LIB = $(LIBNAME).a
DYNAMIC_LIB = $(LIBNAME).so

# Main target
all: $(BIN) 
	@echo "Build Complete: Executable"

static: $(STATIC_LIB) 
	@echo "Build Complete: Dynamic Library"
dynamic: $(DYNAMIC_LIB)
	@echo "Build Complete: Static Library"

# Build executable
$(BIN): $(OBJFILES)
	$(CC) $(CFLAGS) $(MAIN) -o $@ $^

# Build object files
$(OBJFILES): $(CFILES)
	$(CC) $(CFLAGS) -c $^

# Build static library
$(STATIC_LIB): $(OBJFILES)
	ar rcs $@ $^

# Build dynamic library
$(DYNAMIC_LIB): $(OBJFILES)
	$(CC) -shared -o $@ $^

# Clean target
clean:
	rm -f $(OBJFILES) $(BIN) $(STATIC_LIB) $(DYNAMIC_LIB)

