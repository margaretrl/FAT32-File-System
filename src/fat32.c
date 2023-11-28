/*// Fat32 functions
#include "fat32.h"

long clusterToByteOffset(uint32_t clusterNumber, const FAT32VolumeInfo *volInfo) {
    uint32_t firstSectorOfCluster = ((clusterNumber - 2) * volInfo->sectorsPerCluster) + volInfo->firstDataSector;
    long offset = firstSectorOfCluster * volInfo->bytesPerSector;
    return offset;
}



void getCurrentClusterData(uint32_t clusterNumber, FAT32DirEntry *buffer, size_t bufferSize) {
    // Calculate the byte offset of the cluster in the FAT32 image
    long offset = clusterToByteOffset(clusterNumber);

    // Seek to the offset and read the cluster data
    fseek(fat32ImageFile, offset, SEEK_SET);
    fread(buffer, sizeof(FAT32DirEntry), bufferSize, fat32ImageFile);
}

uint32_t getNextCluster(uint32_t currentCluster) {
    uint32_t nextCluster;
    // Calculate the offset in the FAT for the current cluster
    long fatOffset =  Calculate offset for currentCluster in the FAT ;

    // Read the next cluster value from the FAT
    fseek(fat32ImageFile, fatOffset, SEEK_SET);
    fread(&nextCluster, sizeof(uint32_t), 1, fat32ImageFile);

    return nextCluster; // Return the next cluster number
}

bool isEntryValid(const FAT32DirEntry entry) {
    // Check if the entry is deleted or empty
    if (entry.name[0] == DELETED_ENTRY || entry.name[0] == EMPTY_ENTRY) {
        return false;
    }
    return true;
}


DirectoryEntry* getCurrentDirectoryEntries() {
    // Array to hold the parsed directory entries
    DirectoryEntry* entries = malloc(MAX_ENTRIES * sizeof(DirectoryEntry));
    if (entries == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Buffer to hold raw directory data
    FAT32DirEntry rawEntries[MAX_ENTRIES];
    size_t entryIndex = 0;

// Assuming getCurrentClusterData reads data from the current cluster
    while (currentCluster != END_OF_CLUSTERCHAIN && entryIndex < MAX_ENTRIES) {
        getCurrentClusterData(currentCluster, rawEntries, sizeof(rawEntries));

        // Process rawEntries here (in the next step)

        currentCluster = getNextCluster(currentCluster); // Get the next cluster in the chain
    }

    // Read the current directory data into rawEntries
    // You'll need to handle reading from the correct cluster(s)
    // and possibly following the cluster chain if the directory spans multiple clusters

    // Parse each raw entry and fill the `entries` array
    for (size_t i = 0; i < MAX_ENTRIES; ++i) {
        if (isEntryValid(rawEntries[i])) {
            // Convert the 8+3 name to a standard format
            convertFAT32NameToStandard(rawEntries[i].name, entries[entryIndex].name);

            // Set directory flag
            entries[entryIndex].isDirectory = (rawEntries[i].attr & DIRECTORY_ATTRIBUTE) != 0;

            entryIndex++;
        }
    }

    return entries;
}
*/