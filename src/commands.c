void printFileSystemInfo(int BPB_BytesPerSec, int BPB_SecPerClus, int BPB_RsvdSecCnt, int BPB_NumFATs, int BPB_FATSz32) {
    printf("BPB_BytesPerSec : %d\n", BPB_BytesPerSec);
    printf("BPB_BytesPerSec : %x\n", BPB_BytesPerSec);
    printf("\n");

    printf("BPB_SecPerClus : %d\n", BPB_SecPerClus);
    printf("BPB_SecPerClus : %x\n", BPB_SecPerClus);
    printf("\n");

    printf("BPB_RsvdSecCnt : %d\n", BPB_RsvdSecCnt);
    printf("BPB_RsvdSecCnt : %x\n", BPB_RsvdSecCnt);
    printf("\n");

    printf("BPB_NumFATs : %d\n", BPB_NumFATs);
    printf("BPB_NumFATs : %x\n", BPB_NumFATs);
    printf("\n");

    printf("BPB_FATSz32 : %d\n", BPB_FATSz32);
    printf("BPB_FATSz32 : %x\n", BPB_FATSz32);
    printf("\n");
}
