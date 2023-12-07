CC = gcc
CFLAGS = -Wall -g -std=c99
INC = -I include

# Define source and object files
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

# Target executable name and bin directory
TARGET = filesys
BINDIR = bin

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJ)
	mkdir $(BINDIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f src/*.o $(BINDIR)/$(TARGET)
	rm -r $(BINDIR)

.PHONY: all clean
