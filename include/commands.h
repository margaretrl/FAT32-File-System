#ifndef COMMANDS_H
#define COMMANDS_H

#include "fat32.h"
#include <stdio.h>
#include <string.h> // dont know if im allowed to include this

void cdcmd(char* command, int cmdlength);
void infocmd(BootSectorData bootSectorData);
// New function declarations for Part 2
void changeDirectory(FILE *imageFile, BootSectorData bootSectorData, char *path, char *newDir);
void listDirectory(FILE *imageFile, BootSectorData bootSectorData, char *path);
void readDirectoryEntry(FILE *imageFile, BootSectorData bootSectorData, unsigned int cluster, DirectoryEntry *entry);
//void readDirectoryEntry(FILE *imageFile, BootSectorData bootSectorData, unsigned int cluster, DirectoryEntry *entries);
unsigned int getNextCluster(FILE *imageFile, BootSectorData bootSectorData, unsigned int currentCluster);
void readDirectoryEntries(FILE *imageFile, BootSectorData bootSectorData, unsigned int cluster, DirectoryEntry *entries);

#endif