# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -lpthread

# Target executables
TARGETS = visitor receptionist monitor init_smem

# Source files
SRCS = $(wildcard src/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGETS)

# Link object files to create the executables
visitor: src/visitor.o src/smem.o
	$(CC) $(CFLAGS) src/visitor.o src/smem.o -o visitor -lpthread

receptionist: src/receptionist.o src/smem.o
	$(CC) $(CFLAGS) src/receptionist.o src/smem.o -o receptionist -lpthread

monitor: src/monitor.o src/smem.o
	$(CC) $(CFLAGS) src/monitor.o src/smem.o -o monitor -lpthread

init_smem: src/init_smem.o src/smem.o
	$(CC) $(CFLAGS) src/init_smem.o src/smem.o -o init_smem -lpthread

# Compile source files to object files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(TARGETS) src/*.o