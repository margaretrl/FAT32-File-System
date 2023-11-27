#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat32.h"
#include "commands.h"
#include "utils.h"

#define MAX_COMMAND_LENGTH 100

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./filesys [FAT32 ISO]\n");
        return 1;
    }

    char *imagePath = argv[1];
    FILE *fp = fopen(imagePath, "rb+");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", imagePath);
        return 1;
    }

    //readBootSector(fp);

    // Shell loop
    char commandLine[MAX_COMMAND_LENGTH];
    while (1) {
        printf("[%s]> ", imagePath);

        if (fgets(commandLine, MAX_COMMAND_LENGTH, stdin) == NULL) {
            fprintf(stderr, "Error reading command. Exiting.\n");
            break;
        }

        commandLine[strcspn(commandLine, "\n")] = 0;

        // commands
        if (strcmp(commandLine, "exit") == 0) {
            break;
        } else if (strcmp(commandLine, "info") == 0) {
            // infocmd();
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
