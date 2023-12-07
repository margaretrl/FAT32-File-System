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
    char BS_OEMName[8];
    int16_t BPB_BytesPerSec;
    int8_t BPB_SecPerClus;
    int16_t BPB_RsvdSecCnt;
    int8_t BPB_NumFATs;
    int16_t BPB_RootEntCnt;
    char BS_VolLab[11];
    int32_t BPB_FATSz32;
    int32_t BPB_RootClus;
    int32_t RootDirSectors;
    int32_t FirstDataSector;
    int32_t FirstSectorofCluster;
    int32_t root_address;
    //NEW MARGARET
    int32_t TotalSectors;
    long file_size;
} BootSectorData;

struct __attribute__((__packed__)) DirectoryEntry
{
    char DIR_Name[11];
    uint8_t DIR_Attr;
    uint8_t Unused1[8];
    uint16_t DIR_FirstClusterHigh;
    uint8_t Unused2[4];
    uint16_t DIR_FirstClusterLow;
    uint32_t DIR_FileSize;
};

typedef struct {
    char filename[FILENAME_MAX]; // Assuming FILENAME_MAX is defined
    char mode[3]; // Store "-r", "-w", "-rw", or "-wr"
    int offset; // Current offset in the file
    // Add other necessary fields, like cluster number, directory entry info, etc.
} OpenFile;


#endif