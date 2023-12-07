#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "fat32.h"
#include <ctype.h>
#define MAX_SIZE_FILE 16


int LBAToOffset(int sector, BootSectorData bs);
int compare(char IMG_Name[], char input[]);
int match(struct DirectoryEntry dir[], char token[]);


#endif