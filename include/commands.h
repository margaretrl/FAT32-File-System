#ifndef COMMANDS_H
#define COMMANDS_H

#include "fat32.h"
#include <stdio.h>
#include <string.h> // dont know if im allowed to include this

#define MAX_OPEN_FILES 10 // Maximum number of files that can be open at once

void printFileSystemInfo(BootSectorData bs);
void lsfunction(struct DirectoryEntry dir[]);
void ReadDirEntries(struct DirectoryEntry dir[], int counter, FILE *imageFile, BootSectorData bs);
int openFile(const char* filename, const char* mode, int openFilesCount,OpenFile openFiles[MAX_OPEN_FILES]);
int closeFile(const char* filename, int openFilesCount,OpenFile openFiles[MAX_OPEN_FILES]);


#endif