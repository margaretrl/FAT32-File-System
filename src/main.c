#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat32.h"
#include "commands.h"
#include "utils.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_PATH_LENGTH 256

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./filesys [FAT32 ISO]\n");
        return 1;
    }

    char *imagePath = argv[1];
    char imageName[MAX_PATH_LENGTH];
    strncpy(imageName, imagePath, MAX_PATH_LENGTH);
    imageName[MAX_PATH_LENGTH - 1] = '\0';

    FILE *fp = fopen(imagePath, "rb+");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", imagePath);
        return 1;
    }

    BootSector bootSector;
    if (fread(&bootSector, sizeof(BootSector), 1, fp) != 1) {
        fprintf(stderr, "Error: Unable to read file %s i guess\n", imagePath);
        return 1;
    }

    // Shell loop
    char currentPath[MAX_PATH_LENGTH] = "/";
    char commandLine[MAX_COMMAND_LENGTH];
    while (1) {
        printf("[%s%s]> ", imageName, currentPath); //prompt
        if (fgets(commandLine, MAX_COMMAND_LENGTH, stdin) == NULL) {
            fprintf(stderr, "Error reading command. Exiting.\n");
            break;
        }

        commandLine[strcspn(commandLine, "\n")] = 0;

        // commands
        if (strcmp(commandLine, "exit") == 0) {
            break;
        } else if (strcmp(commandLine, "info") == 0) {
            infocmd(&bootSector);
        } else if (strncmp(commandLine, "cd ", 3) == 0) {
            // cdcmd();
        } else if (strcmp(commandLine, "ls") == 0) {
            // lscmd();
        } else {
            printf("Unknown command.\n");
        }
    }

    fclose(fp);
    return 0;
}
