#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define MAX_FILENAME_LENGTH 255
// #define DELETED_ENTRY 0xE5
// #define EMPTY_ENTRY 0x00
#define SECTOR_SIZE 512
#define DIR_ENTRY_SIZE 32
#define MAX_FILENAME_LENGTH 255


typedef struct {
    uint16_t bytesPerSector;
    unsigned int sectorsPerCluster;
    unsigned int firstDataSector;
    unsigned int rootCluster;
    unsigned int rootClusterPosition;
    uint32_t totalClusters;
    unsigned int numEntriesInFAT;
    unsigned int sizeOfImage;
} BootSectorData;

// Function prototype
unsigned int firstSectorOfCluster(unsigned int cluster, const BootSectorData *bootSectorData);
typedef struct {
    char name[MAX_FILENAME_LENGTH];
    unsigned char attributes;
    unsigned int firstCluster;
    unsigned int fileSize;
} DirectoryEntry;


BootSectorData parseBootSector(FILE *imageFile);


/*
typedef struct {
    uint32_t sectorsPerCluster;
    uint32_t firstDataSector;
    uint32_t bytesPerSector;
    // ... other FAT32 parameters ...
} FAT32VolumeInfo;

extern FAT32VolumeInfo fat32VolumeInfo;


typedef struct {
    char name[MAX_FILENAME_LENGTH];
    bool isDirectory;
    // ... other properties ...
} DirectoryEntry;

typedef struct {
    char name[11];  // 8+3 name format
    uint8_t attr;   // Attributes byte
    uint16_t firstClusterHigh; // High bytes of the first cluster
    uint16_t firstClusterLow;  // Low bytes of the first cluster
    // ... other fields as necessary ...
} FAT32DirEntry;

void getCurrentClusterData(uint32_t clusterNumber, FAT32DirEntry *buffer, size_t bufferSize);
uint32_t getNextCluster(uint32_t currentCluster);
bool isEntryValid(const FAT32DirEntry entry);
long clusterToByteOffset(uint32_t clusterNumber);


DirectoryEntry* getCurrentDirectoryEntries();
size_t getNumberOfEntriesInCurrentDirectory();
*/
#endif