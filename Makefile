CC = gcc
CFLAGS = -Wall -g -std=c99
INC = -I include

# Define source and object files
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

# Target executable name and bin directory
TARGET = filesys

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean