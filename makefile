# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -O2 -std=c99 --pedantic
RAYLIB = raylib-5.5_linux_amd64
LDFLAGS = -I ./$(RAYLIB)/include ./$(RAYLIB)/lib/libraylib.a -lm

# Output binary name
TARGET = synt

# Source files
SRC = main.c synt.c
INC = synt.h note.h

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC) $(INC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
