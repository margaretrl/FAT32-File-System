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
    // !!! Need to add checking if the file exists
    // !!! cd check path
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

        strncpy(openFiles[openFilesCount].mode, mode, 3);
        openFiles[openFilesCount].mode[3] = '\0';

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
                openFilesCount--;
                // may be a better way to do this
                openFiles[openFilesCount].filename[0] = '\0';
                openFiles[openFilesCount].mode[0] = '\0';
                openFiles[openFilesCount].offset = '\0';


                return 0;
            }
        }
        return -1;
}

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    char mode[MAX_MODE_LENGTH];
    int offset;
    char path[MAX_PATH_LENGTH]; // This field should store the path
} OpenFile;


void lsoffunction(OpenFile openFiles[MAX_OPEN_FILES])
{
    // !!! still neeed to do formatting stuff for it
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
            // Printing each file's details including the path
            printf("%-10d %-15s %-10s %-10d %s\n", i, openFiles[i].filename, openFiles[i].mode, openFiles[i].offset, openFiles[i].path);
            i++;
        }
        //printf("this is gonna be the lsof function and size is: %lu\n", sizeof(openFiles));
    }
}


void lseekfunction(OpenFile openFiles[], const char *filename, char* token)
{
    int fileFound = 0;
    if (token[1] == NULL || token[2] == NULL) {
        printf("Usage: lseek [FILENAME] [OFFSET]\n");
    }
    else {
        int newOffset = atoi(token[2]);
        for (int i = 0; i < 10; i++) // assuming max 10 open files
        {
            if (strcmp(openFiles[i].filename, filename) == 0 && openFiles[i].filename[0] != '\0') {
                fileFound = 1;

                // Check if the new offset is larger than the file size
                /*
                if (newOffset > openFiles[i].fileSize)
                {
                    printf("Error: Offset is larger than the file size.\n");
                    return;
                }*/

                openFiles[i].offset = newOffset;
                printf("Offset of file '%s' set to %d.\n", filename, newOffset);
                return;
            }
        }

        if (!fileFound) {
            printf("Error: File '%s' is not opened or does not exist.\n", filename);
        }
    }
}
