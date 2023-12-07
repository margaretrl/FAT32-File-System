CC = gcc
CFLAGS = -Wall -g
INC = -I include

# Define source and object files
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

# Target executable name and bin directory
TARGET = filesys
BINDIR = bin

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJ)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f src/*.o $(BINDIR)/$(TARGET)

.PHONY: all clean
