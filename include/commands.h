#ifndef COMMANDS_H
#define COMMANDS_H

#include "fat32.h"
#include <stdio.h>
#include <string.h> // dont know if im allowed to include this

void printFileSystemInfo(BootSectorData bs);
void lsfunction(struct DirectoryEntry dir[]);
void ReadDirEntries(struct DirectoryEntry dir[], int counter, FILE *ptr_file, BootSectorData bs);

#endif