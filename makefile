# Compiler and flags
CC = tcc
IFLAGS = 
CFLAGS = -Wall -Wextra
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

clean:
	rm -f $(TARGET)
