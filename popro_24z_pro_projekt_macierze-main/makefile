# Compiler and flags
CC = gcc

# Target executable name
TARGET = main

# Sources and objects
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	del -f $(OBJS) $(TARGET)

# Phony targets to avoid conflicts
.PHONY: all clean
