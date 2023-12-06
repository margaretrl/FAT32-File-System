#include "fat32.h"


void printFileSystemInfo(BootSectorData bs) {
    printf("Bytes per Sector: %d\n", bs.BPB_BytesPerSec);
    printf("Sectors per Cluster: %d\n", bs.BPB_SecPerClus);

    printf("BPB_RsvdSecCnt : %d\n", bs.BPB_RsvdSecCnt);
    printf("BPB_RsvdSecCnt : %x\n", bs.BPB_RsvdSecCnt);
    printf("\n");

    printf("BPB_NumFATs : %d\n", bs.BPB_NumFATs);
    printf("BPB_NumFATs : %x\n", bs.BPB_NumFATs);
    printf("\n");

    printf("BPB_FATSz32 : %d\n", bs.BPB_FATSz32);
    printf("BPB_FATSz32 : %x\n", bs.BPB_FATSz32);
    printf("\n");
}
