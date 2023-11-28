// Implementation of commands
#include "fat32.h"
#include "commands.h"
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



