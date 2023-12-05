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

#define SECTOR_SIZE 512
#define DIR_ENTRY_SIZE 32
#define MAX_FILENAME_LENGTH 255



//read and parse boot sector
BootSectorData parseBootSector(FILE *imageFile) {
    BootSectorData bootSectorData;
    unsigned char buffer[SECTOR_SIZE];

// Reading the first sector
    fseek(imageFile, 0, SEEK_SET);
    fread(buffer, SECTOR_SIZE, 1, imageFile);

// Parsing necessary fields
    bootSectorData.bytesPerSector = buffer[11] + (buffer[12] << 8);
    bootSectorData.sectorsPerCluster = buffer[13];
    uint32_t reservedSectors = buffer[14] + (buffer[15] << 8);
    uint32_t numFATs = buffer[16];
    uint32_t totalSectors = buffer[32] + (buffer[33] << 8) + (buffer[34] << 16) + (buffer[35] << 24);
    uint32_t fatSize = buffer[36] + (buffer[37] << 8) + (buffer[38] << 16) + (buffer[39] << 24);
    bootSectorData.rootClusterPosition = buffer[44] + (buffer[45] << 8) + (buffer[46] << 16) + (buffer[47] << 24);

// Calculating total clusters and size of image
    uint32_t totalDataSectors = totalSectors - (reservedSectors + (numFATs * fatSize));
    bootSectorData.totalClusters = totalDataSectors / bootSectorData.sectorsPerCluster;
    //bootSectorData.totalClusters = ((fatSize - reservedSectors) / bootSectorData.sectorsPerCluster);
    bootSectorData.numEntriesInFAT = ((fatSize * bootSectorData.bytesPerSector)/ 4);  // Each FAT entry is 4 bytes
    fseek(imageFile, 0, SEEK_END);
    bootSectorData.sizeOfImage = ftell(imageFile);

    return bootSectorData;
}
// Function to display boot sector information
void displayInfo(BootSectorData bootSectorData) {
    printf("Position of Root Cluster: %u\n", bootSectorData.rootClusterPosition);
    printf("Bytes Per Sector: %u\n", bootSectorData.bytesPerSector);
    printf("Sectors Per Cluster: %u\n", bootSectorData.sectorsPerCluster);
    printf("Total Number of Clusters: %u\n", bootSectorData.totalClusters);
    printf("Number of Entries in One FAT: %u\n", bootSectorData.numEntriesInFAT);
    printf("Size of Image: %lu bytes\n", bootSectorData.sizeOfImage);
}

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    unsigned char attributes;
    unsigned int firstCluster;
    unsigned int fileSize;
} DirectoryEntry;


// New function declarations for Part 2
void changeDirectory(FILE *imageFile, BootSectorData bootSectorData, char *path, char *newDir);
void listDirectory(FILE *imageFile, BootSectorData bootSectorData, char *path);
void readDirectoryEntry(FILE *imageFile, BootSectorData bootSectorData, unsigned int cluster, DirectoryEntry *entry);

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
            displayInfo(bootSectorData);
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

// Implementations of new functions for Part 2

void changeDirectory(FILE *imageFile, BootSectorData bootSectorData, char *path, char *newDir) {
    // You need to find the cluster number of 'newDir' in the current directory
    // If 'newDir' is found and it is a directory, update 'path'
    // For simplicity, this function assumes 'newDir' is in the root directory
    // A real implementation should search the current directory

    DirectoryEntry entry;
    // Assuming root directory
    readDirectoryEntry(imageFile, bootSectorData, bootSectorData.rootCluster, &entry);

    // If newDir is found and is a directory, update the path
    // This is a simplified check
    if (strcmp(entry.name, newDir) == 0 && (entry.attributes & 0x10)) {
        strcpy(path, newDir); // Update the path
    } else {
        printf("Directory not found: %s\n", newDir);
    }
}

void listDirectory(FILE *imageFile, BootSectorData bootSectorData, char *path) {
    // List all files and directories in the current directory
    // For simplicity, this function assumes the current directory is the root directory
    // A real implementation should find the correct directory based on 'path'

    DirectoryEntry entry;
    // Assuming root directory for simplicity
    readDirectoryEntry(imageFile, bootSectorData, bootSectorData.rootCluster, &entry);

    // Display the entry - this is a simplified display
    printf("%s\n", entry.name);
}

void readDirectoryEntry(FILE *imageFile, BootSectorData bootSectorData, unsigned int cluster, DirectoryEntry *entry) {
    // Calculate the sector number for the cluster
    unsigned int sectorNum = firstSectorOfCluster(cluster, &bootSectorData);
    unsigned char buffer[SECTOR_SIZE];

    // Read the sector
    fseek(imageFile, sectorNum * SECTOR_SIZE, SEEK_SET);
    fread(buffer, SECTOR_SIZE, 1, imageFile);

    // Parse the directory entry - this is a simplified parsing
    // A real implementation should loop through the buffer and parse each 32-byte directory entry
    strncpy(entry->name, buffer, 11); // Copy the first 11 bytes as the file name
    entry->name[11] = '\0'; // Null-terminate the string
    entry->attributes = buffer[11]; // The attributes byte
}
