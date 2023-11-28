// Implementation of commands
#include "fat32.h"
#include "commands.h"



void infocmd(const BootSector *bs) {
    if (bs == NULL) {
        printf("Error: Boot sector data is not available.\n");
        return;
    }

    printf("Position of Root Cluster: %u\n", bs->rootCluster);
    printf("Bytes Per Sector: %u\n", bs->bytesPerSector);
    printf("Sectors Per Cluster: %u\n", bs->sectorsPerCluster);
    printf("Total Number of Clusters in Data Region: %u\n", bs->totalClusters);
    printf("Number of Entries in One FAT: %u\n", bs->fatEntries);
    printf("Size of Image in Bytes: %u\n", bs->imageSize);
}

