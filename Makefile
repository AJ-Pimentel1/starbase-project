
CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g

# Default target: build the parser test program.
all: parser_test

# Compile the binary map parser.
parser_test: bm_parser.c bm_parser.h
	$(CC) $(CFLAGS) bm_parser.c -o parser_test

# Build and execute the parser test.
test: parser_test
	./parser_test

# Remove generated executables and object files.
clean:
	rm -f parser_test starbase_server *.o

.PHONY: all test clean
