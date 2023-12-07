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
//#define SECTOR_SIZE 512
//#define DIR_ENTRY_SIZE 32
#define MAX_FILENAME_LENGTH 12


typedef struct {
    //char BS_OEMName[8];
    int16_t bytesPerSector;
    int8_t sectorsPerCluster;
    //int16_t rootEntCnt;
    //char volLabel[11];
    int32_t FATSize32;
    int32_t rootClusterNum;
    //int32_t rootDirSectors;
    //int32_t firstDataSector;
    //int32_t firstClusterSector;
    int16_t reservedSectorCnt;
    int8_t FATnum;
    int32_t rootAddress;
    int32_t totalSectors;
    long fileSize;
} BootSectorData;

struct __attribute__((__packed__)) DirectoryEntry
{
    char name[11];
    uint8_t attributes;
    uint8_t PlaceHolder[8]; // Do not remove
    uint16_t firstClusterHigh;
    uint8_t PlaceHolder2[4]; // Do not remove
    uint16_t firstClusterLow;
    uint32_t fileSize;
};

typedef struct {
    char filename[FILENAME_MAX]; // WHY DOES THIS WORK IF FILENAME_MAX ISNT DEFINED!!?
    //Should it just be MAX_FILENAME_LENGTH?
    char mode[3]; // Store "-r", "-w", "-rw", or "-wr"
    int offset; // Current offset in the file
    // Maybe add other necessary fields, like cluster number, directory entry info, etc.
} OpenFile;


#endif