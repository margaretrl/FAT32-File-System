#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "fat32.h"
#include <ctype.h>
#define MAX_FILE_SIZE 16


int LBAToOffset(int sector, BootSectorData bs);
int compare(char imgName[], char input[]);
int match(struct DirectoryEntry dir[], char token[]);
void setFileOffset(OpenFile openFiles[], const char *filename, int newOffset);


#endif