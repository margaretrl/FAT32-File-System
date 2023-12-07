#include "fat32.h"
#include "functions.h"
#define MAX_OPEN_FILES 10 // Maximum number of files that can be open at once






void printFileSystemInfo(BootSectorData bs) {
    printf("Bytes per Sector: %d\n", bs.bytesPerSector);
    printf("Sectors per Cluster: %d\n", bs.sectorsPerCluster);
    printf("Total clusters in Data Region: %d\n", (bs.totalSectors - (bs.reservedSectorCnt + (bs.FATnum * bs.FATSize32))/ bs.sectorsPerCluster));
    printf("# of entries in one FAT: %f\n", ((bs.FATSize32 * bs.bytesPerSector) / 4.));
    printf("Size of Image (bytes): %ld\n",bs.fileSize);
    printf("Root Cluster: %d\n", bs.rootClusterNum);
    printf("First Data Sector: %d\n",bs.reservedSectorCnt + (bs.FATnum * bs.FATSize32));
    printf("Total Sectors: %d\n",bs.totalSectors);
}

void lsfunction(struct DirectoryEntry dir[])
{
    int i = 0;
    while (i < 16)
    {
        char word[12];
        memset(&word, 0, 12);
        //Checks if the fils are read only, subdirectories, 0X30.
        //Does not print the deleted file (signed char)0Xe5.
        if ((dir[i].attributes == 0x01 ||
             dir[i].attributes == 0x10 ||
             dir[i].attributes == 0x20 ||
             dir[i].attributes == 0x30) &&
            dir[i].name[0] != (signed char)0xe5)
        {
            strncpy(word, dir[i].name, 11);
            printf("%s\n", word);
        }
        i++;
    }
}

void ReadDirEntries(struct DirectoryEntry dir[], int counter, FILE *imageFile, BootSectorData bs)
{
    fseek(imageFile, LBAToOffset(dir[counter].firstClusterLow, bs), SEEK_SET);
    int i = 0;
    for (i = 0; i < 16; i++)
    {
        fread(&dir[i], sizeof(dir[i]), 1, imageFile);
    }
}

int openFile(const char* filename, const char* mode, int openFilesCount,OpenFile openFiles[MAX_OPEN_FILES]) {
    // Check if the file is already open
    for (int i = 0; i < openFilesCount; i++) {
        //printf("%s\n",openFiles[i].filename);
        if (strcmp(openFiles[i].filename, filename) == 0) {
            printf("Error: File is already opened.\n");
            return -1;
        }
    }

    // Validate the mode
    if (strcmp(mode, "-r") != 0 && strcmp(mode, "-w") != 0 &&
        strcmp(mode, "-rw") != 0 && strcmp(mode, "-wr") != 0) {
        printf("Error: Invalid mode.\n");
        return -1;
    }

    if (openFilesCount < MAX_OPEN_FILES) {
        strncpy(openFiles[openFilesCount].filename, filename, FILENAME_MAX - 1);
        openFiles[openFilesCount].filename[FILENAME_MAX - 1] = '\0'; // Ensure null-termination

        strncpy(openFiles[openFilesCount].mode, mode, 2);
        openFiles[openFilesCount].mode[2] = '\0';

        openFiles[openFilesCount].offset = 0;
        // Set other necessary file info, like cluster number, etc.

        //openFilesCount++;
        return 0; // Success
    } else {
        printf("Error: Too many open files.\n");
        return -1;
    }
}

int closeFile(const char* filename, int openFilesCount,OpenFile openFiles[MAX_OPEN_FILES]) {
    // Check if file is open
        for (int i = 0; i < openFilesCount; i++) {
            if (strcmp(openFiles[i].filename, filename) == 0) {
                // Shift all elements after the found element one position back
                for (int j = i; j < openFilesCount - 1; j++) {
                    openFiles[j] = openFiles[j + 1];
                }
                return 0;
            }
        }
        return -1;
}