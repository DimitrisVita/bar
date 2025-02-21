# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# Target executables
TARGETS = visitor receptionist monitor smem_init

# Source files
SRCS = $(wildcard src/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGETS)

# Link object files to create the executables
visitor: src/visitor.o src/smem_utils.o
	$(CC) $(CFLAGS) src/visitor.o src/smem_utils.o -o visitor -lpthread -Iinclude

receptionist: src/receptionist.o src/smem_utils.o
	$(CC) $(CFLAGS) src/receptionist.o src/smem_utils.o -o receptionist -lpthread -Iinclude

monitor: src/monitor.o src/smem_utils.o
	$(CC) $(CFLAGS) src/monitor.o src/smem_utils.o -o monitor -lpthread -Iinclude

smem_init: src/smem_init.o src/smem_utils.o
	$(CC) $(CFLAGS) src/smem_init.o src/smem_utils.o -o smem_init -lpthread -Iinclude

# Compile source files to object files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(TARGETS) src/*.o