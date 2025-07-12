# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -O2 -std=c99 --pedantic
LDFLAGS = -lm

# Output binary name
TARGET = synt

# Source files
SRC = main.c synt.c
INC = synt.h note.h

.PHONY: all clean run

all: clean $(TARGET)

$(TARGET): $(SRC) $(INC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)
	aplay output.wav

clean:
	rm -f $(TARGET)
