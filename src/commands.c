// Implementation of commands
#include "fat32.h"
#include "commands.h"
#define SECTOR_SIZE 512
#define DIR_ENTRY_SIZE 32
#define MAX_FILENAME_LENGTH 255
/*
bool isValidDirectory(const char *dirname) {
    // Assume you have a function that gets the list of entries in the current directory
    DirectoryEntry* entries = getCurrentDirectoryEntries();
    size_t numEntries = getNumberOfEntriesInCurrentDirectory();

    for (size_t i = 0; i < numEntries; i++) {
        if (strcmp(entries[i].name, dirname) == 0 && entries[i].isDirectory) {
            return true;
        }
    }

    return false;
}



void updateCurrentPath(const char *newdirectory) {
    // Update the global currentPath variable to newDir
    // Handle concatenation, parent directory (..), and root (/) appropriately
    // Ensure buffer overflow is prevented in currentPath
}
*/
/*
void infocmd(const BootSectorData *bs) {
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
}*/


// Function to display boot sector information
void infocmd(BootSectorData bootSectorData) {
    printf("Bytes Per Sector: %u\n", bootSectorData.bytesPerSector);
    printf("Sectors Per Cluster: %u\n", bootSectorData.sectorsPerCluster);
    printf("Total clusters in Data Region: %u\n", bootSectorData.totalClusters);
    printf("# of entries in one FAT: %u\n", bootSectorData.numEntriesInFAT);
    printf("Size of Image (bytes): %u\n", bootSectorData.sizeOfImage);
    printf("Root Cluster: %u\n", bootSectorData.rootClusterPosition);
}   

void cdcmd(char* command, int cmdlength)
{
    // not handling .. rn
    char *dir;
    if (cmdlength == 2) {
        // In a real shell, this would go to the user's home directory.
        // In FAT32, you might want to define a 'home' directory or just go to root.
        dir = "/";  // Assuming root is the 'home' directory
    }
    else {
        dir = &command[3];
    }

    if (strcmp(dir, ".") == 0) {
        // Stay in the current directory
        return;
    }
    else if (strcmp(dir, "..") == 0) {
        // Logic to go up to the parent directory
        // Update currentPath to reflect parent directory
        // Handle edge case of being in root ("/")
        // ... implementation ...
        return;
    }
    /*
    else if (!isValidDirectory(dir)) {
        fprintf(stderr, "cd: %s: No such file or directory\n", dir);
        return;
    }

    updateCurrentPath(dir);
*/
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
    unsigned int cluster = bootSectorData.rootClusterPosition;

    while (cluster != 0xFFFFFFFF) {
        DirectoryEntry entries[SECTOR_SIZE / DIR_ENTRY_SIZE];
        readDirectoryEntries(imageFile, bootSectorData, cluster, entries);

        for (int i = 0; i < SECTOR_SIZE / DIR_ENTRY_SIZE; ++i) {
            if (entries[i].name[0] == 0x00) {
                return; // End of directory entries
            }

            if (entries[i].name[0] != 0xE5) { // Skip deleted entries
                printf("%s\n", entries[i].name); // Print the entry name
            }
        }

        cluster = getNextCluster(imageFile, bootSectorData, cluster);
    }
}



void readDirectoryEntry(FILE *imageFile, BootSectorData bootSectorData, unsigned int cluster, DirectoryEntry *entry) {
    // Calculate the sector number for the cluster
    unsigned int sectorNum = firstSectorOfCluster(cluster, &bootSectorData);
    unsigned char buffer[SECTOR_SIZE];
    strncpy(entry->name, (const char *)buffer, 11); // Cast buffer to const char *

    // Read the sector
    fseek(imageFile, sectorNum * SECTOR_SIZE, SEEK_SET);
    fread(buffer, SECTOR_SIZE, 1, imageFile);

    // Parse the directory entry - this is a simplified parsing
    // A real implementation should loop through the buffer and parse each 32-byte directory entry
    strncpy(entry->name, buffer, 11); // Copy the first 11 bytes as the file name
    entry->name[11] = '\0'; // Null-terminate the string
    entry->attributes = buffer[11]; // The attributes byte
}


void readDirectoryEntries(FILE *imageFile, BootSectorData bootSectorData, unsigned int cluster, DirectoryEntry *entries) {
    unsigned int sectorNum = firstSectorOfCluster(cluster, &bootSectorData);
    unsigned char buffer[SECTOR_SIZE * bootSectorData.sectorsPerCluster];
    size_t offset = 0;

    // Read all sectors in the cluster
    for (int i = 0; i < bootSectorData.sectorsPerCluster; ++i) {
        fseek(imageFile, (sectorNum + i) * SECTOR_SIZE, SEEK_SET);
        fread(buffer + offset, SECTOR_SIZE, 1, imageFile);
        offset += SECTOR_SIZE;
    }

    // Parse the directory entries
    for (int i = 0; i < SECTOR_SIZE * bootSectorData.sectorsPerCluster; i += DIR_ENTRY_SIZE) {
        memcpy(&entries[i / DIR_ENTRY_SIZE], buffer + i, DIR_ENTRY_SIZE);
    }
}



unsigned int getNextCluster(FILE *imageFile, BootSectorData bootSectorData, unsigned int currentCluster) {
    uint32_t FATOffset = currentCluster * 4; // 4 bytes per FAT entry
    unsigned int FATSecNum = bootSectorData.fatStartSector + (FATOffset / SECTOR_SIZE);
    unsigned int FATEntOffset = FATOffset % SECTOR_SIZE;

    unsigned char buffer[4];
    fseek(imageFile, FATSecNum * SECTOR_SIZE + FATEntOffset, SEEK_SET);
    fread(buffer, sizeof(buffer), 1, imageFile);

    unsigned int nextCluster = *((unsigned int *)buffer) & 0x0FFFFFFF;
    if (nextCluster >= 0x0FFFFFF8) return 0xFFFFFFFF; // End of cluster chain
    return nextCluster;
}

