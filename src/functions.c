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
    char filename[256]; 
    char mode[10];      // -r, -w, etc
    int offset;
    int fileSize;
} FileRecord;

char *custom_strsep(char **stringp, const char *delim) {
    if (*stringp == NULL) { 
        return NULL;
    }

    char *start = *stringp;
    char *end;

    if ((end = strpbrk(start, delim)) != NULL) {
        *end = '\0';  // Replace the delimiter with null terminator
        *stringp = end + 1;
    } else {
        *stringp = NULL;
    }

    return start;
}

char *custom_strdup(const char *s, size_t n) {
    size_t len = strnlen(s, n);
    char *new_str = (char *)malloc(len + 1);

    if (new_str == NULL) {
        return NULL;
    }

    new_str[len] = '\0';
    return (char *)memcpy(new_str, s, len);
}

void truncateToLastSlash(char* path) {
    if (path == NULL || strcmp(path, "/") == 0) {
        return;
    }

    char* lastSlash = strrchr(path, '/');
    if (lastSlash != NULL) {
        if (lastSlash == path) {
            // If the only slash is at the beginning, it's the root directory
            *(lastSlash + 1) = '\0'; // Keep the root slash
        } else {
            *lastSlash = '\0';
        }
    }
}

void appendToPath(char* path, const char* toAppend, int maxLen) {
    if (path == NULL || toAppend == NULL) return;

    // Calculate the remaining space in the path
    int spaceLeft = maxLen - strlen(path) - 1; // -1 for the null terminator

    if (spaceLeft > 1) {
        // Append '/' only if the current path doesn't already end with one
        if (path[strlen(path) - 1] != '/') {
            strncat(path, "/", 1);
            spaceLeft--;
        }
        strncat(path, toAppend, spaceLeft);
    }
}