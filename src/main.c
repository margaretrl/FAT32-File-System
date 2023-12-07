#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "commands.h"
#include "fat32.h"
#include "functions.h"

#define WHITESPACE " \t\n"
#define MAX_FILENAME_LENGTH 12 // Given max filename length
#define MAX_CMD_SIZE 255 // The maximum command-line size
#define MAX_ARG_NUM 5 // Assumption based on implemented cmds
#define MAX_FILE_SIZE 16
//#define SECTOR_SIZE 512
//#define DIR_ENTRY_SIZE 32


// int32_t rootDirSectors = 0;
// int32_t firstDataSector = 0;
// int32_t firstClusterSector = 0;

//#define MAX_OPEN_FILES 10 // Maximum number of files that can be open at once



int img_mounted = 0;
int main(int argc, char *argv[])
{
    //Initialize Directory struct
    struct DirectoryEntry dir[16];
    // Initialize boot sector data struct
    BootSectorData bs;
    char *commands = (char *)malloc(MAX_CMD_SIZE);
    if (argc != 2) {
        // Error in usage
        printf("Usage: ./filesys [FAT32 ISO]\n");
        return 1;
    }

    FILE *imageFile = fopen(argv[1], "rb");
    if (!imageFile) {
        // Image file doesn't exist
        printf("Error: File does not exist.\n");
        return 1;
    }
    else
    {
        //Read in from the file
        fseek(imageFile, 11, SEEK_SET);
        fread(&bs.bytesPerSector, 2, 1, imageFile);

        fseek(imageFile, 13, SEEK_SET);
        fread(&bs.sectorsPerCluster, 1, 1, imageFile);

        fseek(imageFile, 14, SEEK_SET);
        fread(&bs.reservedSectorCnt, 1, 2, imageFile);

        fseek(imageFile, 16, SEEK_SET);
        fread(&bs.FATnum, 1, 1, imageFile);

        fseek(imageFile, 36, SEEK_SET);
        fread(&bs.FATSize32, 1, 4, imageFile);

        bs.rootAddress = (bs.bytesPerSector * bs.reservedSectorCnt) +
                       (bs.FATnum * bs.FATSize32 * bs.bytesPerSector);

        fseek(imageFile, bs.rootAddress, SEEK_SET);

        // Saving curr pos of file ptr cus idk
        long current_position = ftell(imageFile); 

        fseek(imageFile, 44, SEEK_SET);
        fread(&bs.rootClusterNum, 4, 1, imageFile);

        fseek(imageFile, 32, SEEK_SET);
        fread(&bs.totalSectors, 4, 1, imageFile);

        fseek(imageFile, 0, SEEK_END);
        long fileSize = ftell(imageFile);
        bs.fileSize = fileSize;

        fseek(imageFile, current_position, SEEK_SET);

        for (int i = 0; i < 16; i++)
        {
            fread(&dir[i], sizeof(dir[i]), 1, imageFile);
        }

        img_mounted = 1;
    }

    // part 2
    char currentPath[MAX_FILENAME_LENGTH] = "/"; // Root directory to start
    //char parameter[100];
    OpenFile openFiles[MAX_OPEN_FILES];

    // Initializing elements of openFiles
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        openFiles[i].filename[0] = '\0'; // Set the first character to null terminator
        openFiles[i].mode[0] = '\0';     // Same for mode
        openFiles[i].offset = 0;         // Initialize offset to 0
    }

    int openFilesCount = 0; // Current number of open files
    while (1)
    {
        // Print out the mfs prompt
        printf("[%s]%s> ", argv[1], currentPath);



        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_CMD_SIZE
        // This while command will wait here until the user
        // inputs something since fgets returns NULL when there
        // is no input
        while (!fgets(commands, MAX_CMD_SIZE, stdin))
            ;

        /* Parse input */
        char *token[MAX_ARG_NUM];

        int token_count = 0;

        // Pointer to point to the token
        // parsed by strsep
        char *arg_ptr;

        char *working_str = strdup(commands);

        // we are going to move the working_str pointer so
        // keep track of its original value so we can deallocate
        // the correct amount at the end
        char *working_root = working_str;

        // Tokenize the input stringswith whitespace used as the delimiter
        while (((arg_ptr = strsep(&working_str, WHITESPACE)) != NULL) &&
               (token_count < MAX_ARG_NUM))
        {
            token[token_count] = strndup(arg_ptr, MAX_CMD_SIZE);
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


            //Closes the file that is open


        //Exits from the img shell
        else if (strcmp("exit", token[0]) == 0)
        {
            if (img_mounted == 1)
            {
                fclose(imageFile);
                imageFile = NULL;
            }
            break;
        }

            //info, stat, ls, get, cd, read works only when the fat32 image is open.
        else if (((strcmp("info", token[0]) == 0) || (strcmp("ls", token[0]) == 0) ||
                  (strcmp("cd", token[0]) == 0) || (strcmp("open", token[0]) == 0) ||
                  (strcmp("close", token[0]) == 0) || (strcmp("lsof", token[0]) == 0) ||
                  (strcmp("lseek", token[0]) == 0) || (strcmp("read", token[0]) == 0))
                  && (img_mounted == 0))
        {
            printf("Error: Image must be mounted first.\n");
        }
        else if (((strcmp("info", token[0]) == 0) || (strcmp("ls", token[0]) == 0) ||
                  (strcmp("cd", token[0]) == 0) || (strcmp("open", token[0]) == 0) ||
                  (strcmp("close", token[0]) == 0) || (strcmp("lsof", token[0]) == 0) ||
                 (strcmp("lseek", token[0]) == 0) || (strcmp("read", token[0]) == 0))
                 && (img_mounted == 1))
        {
            //Prints out all the information abou the fat32 image file.
            if (strcmp("info", token[0]) == 0)
            {
                printFileSystemInfo(bs);
                continue;
            }
            /*
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
                    printf("%d\t\t%d\t%d\n\n", dir[index_counter].attributes,
                           dir[index_counter].fileSize, dir[index_counter].firstClusterLow);
                }
                continue;
            }*/
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
                            if (strstr(dir[counter].name, token[1]) != NULL)
                            {
                                if (dir[counter].firstClusterLow == 0)
                                {
                                    dir[counter].firstClusterLow = 2;
                                }
                                ReadDirEntries(dir, counter, imageFile, bs);
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

                            if (dir[counter].attributes != 0x20 && compare(dir[counter].name, word))
                            {
                                // Directory is found
                                ReadDirEntries(dir, counter, imageFile, bs);
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
            else if (strcmp("open", token[0]) == 0)
            {
                //printf("%d\n",openFilesCount);

                //Check if there is no file name
                if (token[1] == NULL || token[2] == NULL)
                {
                    printf("Error - Usage: open [FILENAME] [FLAGS]\n");
                    continue;
                }
                else
                {
                    int result = openFile(token[1],token[2],openFilesCount,openFiles); //token[2] is hardcoded in atm!!
                    if (result == 0) { // Assuming 0 is the success code
                        // Check if the file is in the openFiles array
                        openFilesCount++;
                        for (int i = 0; i < openFilesCount; i++) {
                            //printf("%s\n",openFiles[i].filename);
                            if (strcmp(openFiles[i].filename, token[1]) == 0) {
                                printf("File is successfully opened.\n");
                                //openFilesCount++;
                                break;
                            }
                        }

                    } else {
                        printf("Failed to open file.\n");
                    }

                    /*
                    imageFile = fopen(token[1], "r");
                    //Check if the given file name exists
                    if (imageFile == NULL)
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
                        fseek(imageFile, 11, SEEK_SET);
                        fread(&bs.bytesPerSector, 2, 1, imageFile);

                        fseek(imageFile, 13, SEEK_SET);
                        fread(&bs.sectorsPerCluster, 1, 1, imageFile);

                        fseek(imageFile, 14, SEEK_SET);
                        fread(&bs.reservedSectorCnt, 1, 2, imageFile);

                        fseek(imageFile, 16, SEEK_SET);
                        fread(&bs.FATnum, 1, 1, imageFile);

                        fseek(imageFile, 36, SEEK_SET);
                        fread(&bs.FATSize32, 1, 4, imageFile);

                        bs.rootAddress = (bs.bytesPerSector * bs.reservedSectorCnt) +
                                       (bs.FATnum * bs.FATSize32 * bs.bytesPerSector);

                        fseek(imageFile, bs.rootAddress, SEEK_SET);
                        int i = 0;
                        for (i = 0; i < 16; i++)
                        {
                            fread(&dir[i], sizeof(dir[i]), 1, imageFile);
                        }

                        printf("File successfully opened!!\n");
                        open_file = 1;
                    }*/
                }
            }
            else if (strcmp("close", token[0]) == 0)
            {
                // if (open_file == 1)
                // {
                //     fclose(imageFile);
                //     imageFile = NULL;
                //     printf("File successfully closed!!\n");
                //     open_file = 0;
                // }
                // else
                // {
                //     printf("Error: File system not open.\n");
                // }
                // continue;

                // Cmd use check
                if(token[1] == NULL ){
                    printf("Error - Usage: close [FILENAME]\n");
                    continue;
                }
                else{
                    if(closeFile(token[1], openFilesCount, openFiles) == 0){
                        printf("File sucessfully closed\n");
                        openFilesCount--; // Decrement the count of open files
                        // !!! i dont think its updateing the array openFiles tho and we gotta like check if its in the list of opened files
                    }
                    else{
                        printf("Error: File is not open.\n");
                    }
                }
            }
            else if (strcmp("lsof", token[0]) == 0)
            {
                lsoffunction(openFiles);

            }
            else if (strcmp("lseek", token[0]) == 0)
            {
                printf("this is gonna be the lseek function\n");

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
                        int bytesNum= atoi(token[2]);

                        int cluster = dir[index_counter].firstClusterLow;

                        fseek(imageFile, position + LBAToOffset(cluster, bs), SEEK_SET);

                        char *temp_str = malloc(bytesNum);

                        fread(temp_str,bytesNum,1,imageFile);

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

