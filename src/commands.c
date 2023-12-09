#include "fat32.h"
#include "functions.h"
#define MAX_OPEN_FILES 10 // Maximum number of files that can be open at once
#define MAX_FILENAME_LENGTH 12


void printFileSystemInfo(BootSectorData bs) {
    printf("Bytes per Sector: %d\n", bs.bytesPerSector);
    printf("Sectors per Cluster: %d\n", bs.sectorsPerCluster);
    printf("Total clusters in Data Region: %d\n", (bs.totalSectors - (bs.reservedSectorCnt +
      (bs.FATnum * bs.FATSize32))/ bs.sectorsPerCluster));
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
        //Checks if the files are read only, subdirectories, 0X30.
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

int openFile(struct DirectoryEntry dir[], const char* filename, const char* mode, 
int openFilesCount,OpenFile openFiles[MAX_OPEN_FILES], char* currentPath) {
    // Check if the file is already open
    for (int i = 0; i < openFilesCount; i++) {
        if (strcmp(openFiles[i].filename, filename) == 0) {
            printf("Error: %s is already opened.\n", filename);
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
        // Possibly need a check for it match doesn't work
        openFiles[openFilesCount].fileSize = dir[match(dir, filename)].fileSize;

        strncpy(openFiles[openFilesCount].filename, filename, MAX_FILENAME_LENGTH - 1);

        openFiles[openFilesCount].filename[FILENAME_MAX - 1] = '\0'; // Ensure null-termination

        strncpy(openFiles[openFilesCount].mode, mode, 3);
        openFiles[openFilesCount].mode[3] = '\0';

        openFiles[openFilesCount].offset = 0;
        
        strncpy(openFiles[openFilesCount].path, currentPath, 11); 

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
                openFilesCount--;
                openFiles[openFilesCount].filename[0] = '\0';
                openFiles[openFilesCount].mode[0] = '\0';
                openFiles[openFilesCount].offset = '\0';
                openFiles[openFilesCount].fileSize = '\0';
                return 0;
            }
        }
        return -1;
}



void lsoffunction(OpenFile openFiles[MAX_OPEN_FILES], char* img_mounted_name)
{
    if (openFiles[0].filename[0] == '\0')
    {
        printf("No files currently open.\n");
    }
    else
    {       //print header 
            printf("%-10s %-15s %-10s %-10s %s\n", "INDEX", "NAME", "MODE", "OFFSET", "PATH");

        int i=0;
        while ((i < MAX_OPEN_FILES) && (openFiles[i].filename[0] != '\0'))
        {

            printf("%-10d %-15s %-10s %-10d %s%s\n", i, openFiles[i].filename,
             openFiles[i].mode, openFiles[i].offset, img_mounted_name,openFiles[i].path);
            i++;
        }
    }
}


void lseekfunction(OpenFile openFiles[], const char *filename, char *offset) {
    int fileFound = -1;
    if (filename == NULL || offset == NULL) {
        printf("Usage: lseek [FILENAME] [OFFSET]\n");
    } else {
        for (int i = 0; i < 10; i++) // assuming max 10 open files
        {
            if (strcmp(openFiles[i].filename, filename) == 0 
             && openFiles[i].filename[0] != '\0') {
                fileFound = i;
            }
        }

        if (fileFound != -1) {
            int newOffset = atoi(offset);

            if (newOffset > openFiles[fileFound].fileSize) {
                printf("Error: Reached end of file.\n");
                return;
            }

            openFiles[fileFound].offset += newOffset;
            return;

        }
        else
        {
            printf("File must be opened first\n");
        }

    if (!fileFound) {
        printf("Error: File '%s' is not opened or does not exist.\n", filename);
    }
}
}
