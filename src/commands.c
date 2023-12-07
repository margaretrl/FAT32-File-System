#include "fat32.h"
#include "functions.h"




void printFileSystemInfo(BootSectorData bs) {
    printf("Bytes per Sector: %d\n", bs.BPB_BytesPerSec);
    printf("Sectors per Cluster: %d\n", bs.BPB_SecPerClus);
    printf("Total clusters in Data Region: %d\n", (bs.TotalSectors - (bs.BPB_RsvdSecCnt + (bs.BPB_NumFATs * bs.BPB_FATSz32))/ bs.BPB_SecPerClus));
    printf("# of entries in one FAT: %f\n", ((bs.BPB_FATSz32 * bs.BPB_BytesPerSec) / 4.));
    printf("Size of Image (bytes): %ld\n",bs.file_size);
    printf("Root Cluster: %d\n", bs.BPB_RootClus);
    printf("First Data Sector: %d\n",bs.BPB_RsvdSecCnt + (bs.BPB_NumFATs * bs.BPB_FATSz32));
    printf("Total Sectors: %d\n",bs.TotalSectors);
    
    //printf("\n");

    // printf("BPB_RsvdSecCnt : %d\n", bs.BPB_RsvdSecCnt);
    // printf("BPB_RsvdSecCnt : %x\n", bs.BPB_RsvdSecCnt);
    // printf("BPB_NumFATs : %d\n", bs.BPB_NumFATs);
    // printf("BPB_NumFATs : %x\n", bs.BPB_NumFATs);
    // printf("\n");

    // printf("BPB_FATSz32 : %d\n", bs.BPB_FATSz32);
    // printf("BPB_FATSz32 : %x\n", bs.BPB_FATSz32);
    // printf("\n");

    // printf("\n"); 
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
        if ((dir[i].DIR_Attr == 0x01 ||
             dir[i].DIR_Attr == 0x10 ||
             dir[i].DIR_Attr == 0x20 ||
             dir[i].DIR_Attr == 0x30) &&
            dir[i].DIR_Name[0] != (signed char)0xe5)
        {
            strncpy(word, dir[i].DIR_Name, 11);
            printf("%s\n", word);
        }
        i++;
    }
}

void ReadDirEntries(struct DirectoryEntry dir[], int counter, FILE *ptr_file, BootSectorData bs)
{
    fseek(ptr_file, LBAToOffset(dir[counter].DIR_FirstClusterLow, bs), SEEK_SET);
    int i = 0;
    for (i = 0; i < 16; i++)
    {
        fread(&dir[i], sizeof(dir[i]), 1, ptr_file);
    }
}
