#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "commands.h"
#include "fat32.h"
#include "functions.h"

#define WHITESPACE " \t\n"
#define MAX_COMMAND_SIZE 255 // The maximum command-line size
#define MAX_NUM_ARGUMENTS 5 // Mav shell only supports five arguments
#define MAX_SIZE_FILE 16
#define SECTOR_SIZE 512
#define DIR_ENTRY_SIZE 32
#define MAX_FILENAME_LENGTH 255

int32_t RootDirSectors = 0;
int32_t FirstDataSector = 0;
int32_t FirstSectorofCluster = 0;

int open_file = 0;
int main(int argc, char *argv[])
{
    //Initialize Directory struct
    struct DirectoryEntry dir[16];
    // Initialize boot sector data struct
    BootSectorData bs;
    char *cmd_str = (char *)malloc(MAX_COMMAND_SIZE);
    if (argc != 2) {
        // Error in usage
        printf("Usage: ./filesys [FAT32 ISO]\n");
        return 1;
    }

    FILE *ptr_file = fopen(argv[1], "rb");
    if (!ptr_file) {
        // Image file doesn't exist
        printf("Error: File does not exist.\n");
        return 1;
    }
    else
    {
        //Read in from the file
        fseek(ptr_file, 11, SEEK_SET);
        fread(&bs.BPB_BytesPerSec, 2, 1, ptr_file);

        fseek(ptr_file, 13, SEEK_SET);
        fread(&bs.BPB_SecPerClus, 1, 1, ptr_file);

        fseek(ptr_file, 14, SEEK_SET);
        fread(&bs.BPB_RsvdSecCnt, 1, 2, ptr_file);

        fseek(ptr_file, 16, SEEK_SET);
        fread(&bs.BPB_NumFATs, 1, 1, ptr_file);

        fseek(ptr_file, 36, SEEK_SET);
        fread(&bs.BPB_FATSz32, 1, 4, ptr_file);

        bs.root_address = (bs.BPB_BytesPerSec * bs.BPB_RsvdSecCnt) +
                       (bs.BPB_NumFATs * bs.BPB_FATSz32 * bs.BPB_BytesPerSec);

        fseek(ptr_file, bs.root_address, SEEK_SET);
        int i = 0;
        for (i = 0; i < 16; i++)
        {
            fread(&dir[i], sizeof(dir[i]), 1, ptr_file);
        }

        open_file = 1;
    }
    char command[100];
    char imagePath[256] = "/";  // Initial path

    // part 2
    char currentPath[MAX_FILENAME_LENGTH] = "/"; // Root directory to start
    char parameter[100];
    while (1)
    {
        // Print out the mfs prompt
        printf("[%s]%s> ", argv[1], currentPath);

        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_COMMAND_SIZE
        // This while command will wait here until the user
        // inputs something since fgets returns NULL when there
        // is no input
        while (!fgets(cmd_str, MAX_COMMAND_SIZE, stdin))
            ;

        /* Parse input */
        char *token[MAX_NUM_ARGUMENTS];

        int token_count = 0;

        // Pointer to point to the token
        // parsed by strsep
        char *arg_ptr;

        char *working_str = strdup(cmd_str);

        // we are going to move the working_str pointer so
        // keep track of its original value so we can deallocate
        // the correct amount at the end
        char *working_root = working_str;

        // Tokenize the input stringswith whitespace used as the delimiter
        while (((arg_ptr = strsep(&working_str, WHITESPACE)) != NULL) &&
               (token_count < MAX_NUM_ARGUMENTS))
        {
            token[token_count] = strndup(arg_ptr, MAX_COMMAND_SIZE);
            if (strlen(token[token_count]) == 0)
            {
                token[token_count] = NULL;
            }
            token_count++;
        }

        //Continues if there is no user input, this checks for fragmentation
        if (token[0] == NULL)
        {
            continue;
        }

            //opens the file
        else if (strcmp("open", token[0]) == 0)
        {
            //Check if there is no file name
            if (token[1] == NULL)
            {
                printf("Error: Please enter the name of file to open.\n");
                continue;
            }
            else
            {
                ptr_file = fopen(token[1], "r");
                //Check if the given file name exists
                if (ptr_file == NULL)
                {
                    printf("Error: File system image not found.\n");
                    continue;
                }
                    //Check if the file is already open
                else if (open_file == 1)
                {
                    printf("Error: File system image already open.\n");
                    continue;
                }
                else
                {
                    //Read in from the file
                    fseek(ptr_file, 11, SEEK_SET);
                    fread(&bs.BPB_BytesPerSec, 2, 1, ptr_file);

                    fseek(ptr_file, 13, SEEK_SET);
                    fread(&bs.BPB_SecPerClus, 1, 1, ptr_file);

                    fseek(ptr_file, 14, SEEK_SET);
                    fread(&bs.BPB_RsvdSecCnt, 1, 2, ptr_file);

                    fseek(ptr_file, 16, SEEK_SET);
                    fread(&bs.BPB_NumFATs, 1, 1, ptr_file);

                    fseek(ptr_file, 36, SEEK_SET);
                    fread(&bs.BPB_FATSz32, 1, 4, ptr_file);

                    bs.root_address = (bs.BPB_BytesPerSec * bs.BPB_RsvdSecCnt) +
                                   (bs.BPB_NumFATs * bs.BPB_FATSz32 * bs.BPB_BytesPerSec);

                    fseek(ptr_file, bs.root_address, SEEK_SET);
                    int i = 0;
                    for (i = 0; i < 16; i++)
                    {
                        fread(&dir[i], sizeof(dir[i]), 1, ptr_file);
                    }

                    printf("File successfully opened!!\n");
                    open_file = 1;
                }
            }
        }

            //Closes the file that is open
        else if (strcmp("close", token[0]) == 0)
        {
            if (open_file == 1)
            {
                fclose(ptr_file);
                ptr_file = NULL;
                printf("File successfully closed!!\n");
                open_file = 0;
            }
            else
            {
                printf("Error: File system not open.\n");
            }
            continue;
        }

            //Exits from the mfs shell
        else if (strcmp("exit", token[0]) == 0)
        {
            if (open_file == 1)
            {
                fclose(ptr_file);
            }
            break;
        }

            //info, stat, ls, get, cd, read works only when the fat32 image is open.
        else if (((strcmp("info", token[0]) == 0) || (strcmp("stat", token[0]) == 0) ||
                  (strcmp("ls", token[0]) == 0) || (strcmp("cd", token[0]) == 0) ||
                  (strcmp("get", token[0]) == 0) || (strcmp("read", token[0]) == 0)) &&
                 (open_file == 0))
        {
            printf("Error: File system must be opened first.\n");
        }
        else if (((strcmp("info", token[0]) == 0) || (strcmp("stat", token[0]) == 0) ||
                  (strcmp("ls", token[0]) == 0) || (strcmp("cd", token[0]) == 0) ||
                  (strcmp("get", token[0]) == 0) || (strcmp("read", token[0]) == 0)) &&
                 (open_file == 1))
        {
            //Prints out all the information abou the fat32 image file.
            if (strcmp("info", token[0]) == 0)
            {
                printFileSystemInfo(bs);
                continue;
            }

                //Prints out the attribute, size, and lower cluster number
                //for the specified folder or file.
            else if (strcmp("stat", token[0]) == 0)
            {
                if (token[1] == NULL)
                {
                    printf("Please specify the name of the directory.\n");
                }
                int index_counter = match(dir, token[1]);
                if (index_counter == -2)
                {
                    printf("Error: File not found.\n");
                } else {
                    printf("Attribute\tSize\tStarting Cluster Number\n");
                    printf("%d\t\t%d\t%d\n\n", dir[index_counter].DIR_Attr,
                           dir[index_counter].DIR_FileSize, dir[index_counter].DIR_FirstClusterLow);
                }
                continue;
            }
                //Implementing ls function
                //List downs the files from a current directory when fat32 image file is open.
            else if (strcmp("ls", token[0]) == 0)
            {
                lsfunction(dir);
                continue;
            }
            else if (strcmp("cd", token[0]) == 0)
            {
                // Change Directory Command
                // Set find bool originally to false
                int find = 0;
                if (token[1] == NULL)
                {
                    // No directory entered
                    printf("Error: Please enter the name of the directory.\n");
                }
                else
                {
                    int counter = 0;
                    if (!strcmp(token[1], "..") || !strcmp(token[1], "."))
                    {
                        // Directory is ".." or "."
                        while (counter < 16)
                        {
                            if (strstr(dir[counter].DIR_Name, token[1]) != NULL)
                            {
                                if (dir[counter].DIR_FirstClusterLow == 0)
                                {
                                    dir[counter].DIR_FirstClusterLow = 2;
                                }
                                ReadDirEntries(dir, counter, ptr_file, bs);
                                break;
                            }
                            counter++;
                        }
                    }
                    else
                    {
                        // Directory entered is not ".." or "."
                        while (counter < 16)
                        {
                            char word[100];
                            // Clear out word
                            memset(word, 0, sizeof(word));
                            strncpy(word, token[1], strlen(token[1]));

                            if (dir[counter].DIR_Attr != 0x20 && compare(dir[counter].DIR_Name, word))
                            {
                                // Directory is found
                                ReadDirEntries(dir, counter, ptr_file, bs);
                                find = 1;
                                break;
                            }
                            counter++;
                        }
                        if (find == 0)
                        {
                            // Directory entered does not exist
                            printf("Error: No such file or directory.\n");
                        }
                    }
                }
                continue;
            }
            else if (strcmp("get", token[0]) == 0)
            {
                if (token[1] == NULL)
                {
                    printf("Please enter the name of the file in the ");
                    printf("following format: get <filename>.\n");
                }
                else
                {
                    int index_counter = match(dir, token[1]);
                    if (index_counter == -2)
                    {
                        printf("Error: File not found.\n");
                    }
                    else
                    {
                        int cluster = dir[index_counter].DIR_FirstClusterLow;
                        int size = dir[index_counter].DIR_FileSize;
                        FILE *file_ptr = fopen(token[1], "w");
                        fseek(ptr_file, LBAToOffset(cluster, bs), SEEK_SET);
                        char *temp_ptr = malloc(size);
                        fread(temp_ptr, size, 1, ptr_file);
                        fwrite(temp_ptr, size, 1, file_ptr);
                        free(temp_ptr);
                        fclose(file_ptr);
                    }
                }
            }

            else if (strcmp("read", token[0]) == 0)
            {
                if (token[1] == NULL || token[2] == NULL )
                {
                    printf("Please enter the file to read in the following format: ");
                    printf("read <filename> <size(number of bytes)>.\n");
                }
                else
                {
                    int index_counter= match(dir,token[1]);
                    if(index_counter==-2)
                    {
                        printf("Error: File not found \n");

                    }
                    else
                    {
                        int position = 0;
                        int NumOfBytes= atoi(token[2]);

                        int cluster = dir[index_counter].DIR_FirstClusterLow;

                        fseek(ptr_file, position + LBAToOffset(cluster, bs), SEEK_SET);

                        char *temp_str = malloc(NumOfBytes);

                        fread(temp_str,NumOfBytes,1,ptr_file);

                        printf("%s\n",temp_str);

                        free(temp_str);

                    }
                }
            }
        }
        else
        {
            printf("Error: Invalid Command.\n");
            continue;
        }
        free(working_root);
    }
    return 0;
}

