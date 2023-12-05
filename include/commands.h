#ifndef COMMANDS_H
#define COMMANDS_H

#include "fat32.h"
#include <stdio.h>
#include <string.h> // dont know if im allowed to include this


//bool isValidDirectory(const char *dirname);
//void updateCurrentPath(const char *newDir);
void infocmd(const BootSectorData *bs);
void cdcmd(char* command, int cmdlength);


#endif
