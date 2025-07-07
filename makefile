# Compiler and flags
CC = gcc
IFLAGS = 
CFLAGS = -Wall -Wextra -O2 --static
LDFLAGS =  -lm

# Output binary name
TARGET = synt

# Source files
SRC = main.c

.PHONY: all clean run

all: clean $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(IFLAGS) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)
	aplay output.wav

clean:
	rm -f $(TARGET)
