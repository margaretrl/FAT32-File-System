#include "functions.h"
#include "fat32.h"


int LBAToOffset(int sector, BootSectorData bs)
{
    return ((sector - 2) * bs.BPB_BytesPerSec) +
           (bs.BPB_BytesPerSec * bs.BPB_RsvdSecCnt) +
           (bs.BPB_NumFATs * bs.BPB_FATSz32 * bs.BPB_BytesPerSec);
}