#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>

typedef struct {
    uint32_t rootCluster;       // Position of root cluster
    uint16_t bytesPerSector;    // Bytes per sector
    uint8_t sectorsPerCluster;  // Sectors per cluster
    uint32_t totalClusters;     // Total number of clusters in data region
    uint32_t fatEntries;        // Number of entries in one FAT
    uint32_t imageSize;         // Size of image in bytes
    // ... other fields as necessary ...
} BootSector;

#endif