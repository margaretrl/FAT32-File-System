#include "functions.h"
#include "fat32.h"

int LBAToOffset(int sector, BootSectorData bs)
{
    return ((sector - 2) * bs.bytesPerSector) +
           (bs.bytesPerSector * bs.reservedSectorCnt) +
           (bs.FATnum * bs.FATSize32 * bs.bytesPerSector);
}

int compare(char imgName[], char input[])
{
    char expanded_name[12];
    memset(expanded_name, ' ', 12);

    char temp[12];

    strcpy(temp, input);

    char *token = strtok(temp, ".");

    if (token == NULL)
    {
        strncpy(expanded_name, "..", strlen(".."));
    }
    else
    {
        strncpy(expanded_name, token, strlen(token));
    }

    token = strtok(NULL, ".");

    if (token)
    {
        strncpy((char *)(expanded_name + 8), token, strlen(token));
    }

    expanded_name[11] = '\0';

    int i;
    for (i = 0; i < 11; i++)
    {
        expanded_name[i] = toupper(expanded_name[i]);
    }


    if (strncmp(expanded_name, imgName, 11) == 0)
    {
        return 1;
    }

    return 0;
}

int match(struct DirectoryEntry dir[], char token[])
{
    int index = 0;
    while (index < MAX_FILE_SIZE)
    {
        if ((dir[index].name[0] != 0xffffffe5) &&
            (compare(dir[index].name, token)) &&
            (dir[index].attributes == 0x01 ||
             dir[index].attributes == 0x10 ||
             dir[index].attributes == 0x20 ||
             dir[index].name[0] == 0x2e))
        {
            return index;
        }
        index++;
    }
    return -2;
}
typedef struct {
    char filename[256]; // or another suitable length
    char mode[10];      // e.g., "r", "w"
    int offset;
    int fileSize;
} FileRecord;

