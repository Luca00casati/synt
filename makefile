# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm

# Output binary name
TARGET = synt

# Source files
SRC = main.c

.PHONY: all clean run

all: clean $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)
	aplay output.wav

clean:
	rm -f $(TARGET)
