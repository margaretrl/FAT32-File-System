/*

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat32.h"
#include "commands.h"
#include "utils.h"
#include <inttypes.h>

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

    FILE *fp = fopen(imagePath, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", imagePath);
        fclose(fp);
        return 1;
    }
    fseek(fp, 0, SEEK_SET);

    BootSector bootSector;
    if (fread(&bootSector, sizeof(BootSector), 1, fp) != 1) {
        fprintf(stderr, "Error: Unable to read file %s i guess\n", imagePath);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    printf("File size: %ld bytes\n", fileSize);


    // bootsector conversion stuff


    bootSector.rootCluster = toLittleEndian32(bootSector.rootCluster);       // Position of root cluster
    bootSector.bytesPerSector = toLittleEndian16(bootSector.bytesPerSector);    // Bytes per sector
    bootSector.sectorsPerCluster;  // Sectors per cluster
    bootSector.totalClusters = toLittleEndian32(bootSector.totalClusters);     // Total number of clusters in data region
    bootSector.fatEntries = toLittleEndian32(bootSector.fatEntries);        // Number of entries in one FAT
    bootSector.imageSize = toLittleEndian32(bootSector.imageSize);




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
        } else if (strncmp(commandLine, "cd ", 2) == 0) {
            //char directory[MAX_PATH_LENGTH];
            //sscanf(commandLine, "cd %s", directory);
            cdcmd(commandLine, strlen(commandLine));
        } else if (strcmp(commandLine, "ls") == 0) {
            // lscmd();
        } else {
            printf("Unknown command.\n");
        }
    }

    fclose(fp);
    return 0;
}


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat32.h"
#include "commands.h"

#define SECTOR_SIZE 512
#define DIR_ENTRY_SIZE 32
#define MAX_FILENAME_LENGTH 255


// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./filesys [FAT32 ISO]\n");
        return 1;
    }

    FILE *imageFile = fopen(argv[1], "rb");
    if (!imageFile) {
        printf("Error: File does not exist.\n");
        return 1;
    }

    BootSectorData bootSectorData = parseBootSector(imageFile);

    char command[100];
    char imagePath[256] = "/";  // Initial path

    // part 2
    char currentPath[MAX_FILENAME_LENGTH] = "/"; // Root directory to start
    char parameter[100];

    while (1) {
        printf("[%s]%s> ", argv[1], currentPath);
        scanf("%s", command);

        if (strcmp(command, "info") == 0) {
            infocmd(bootSectorData);
        } else if (strcmp(command, "cd") == 0) {
            scanf("%s", parameter);
            changeDirectory(imageFile, bootSectorData, currentPath, parameter);
        } else if (strcmp(command, "ls") == 0) {
            listDirectory(imageFile, bootSectorData, currentPath);
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Unknown command.\n");
        }
    }

    fclose(imageFile);
    return 0;
}
