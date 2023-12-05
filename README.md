# FAT32 File System

The purpose of this project is to familiarize you with basic file-system design and implementation. You will need to understand various aspects of the FAT32 file system, such as cluster-based storage, FAT tables, sectors, and directory structure.

## Group Members
- **Margaret Rivas**: mer20c@fsu.edu
- **Sophia Quinoa**: saq20a@fsu.edu
- **Hannah Housand**: hjh21a@fsu.edu

## Division of Labor

### Part 1: Mount the Image File [5 points]
- **Responsibilities**:
- [ ] Mount the image file through command line arguments (./filesys [FAT32 ISO]).
- [ ] Read the image file and implement the correct structure to store and navigate the FAT32 format.
- [ ] Close the program and return an error message if the file does not exist.
- [ ] Implement info command to parse and display boot sector data.
- [ ] Implement exit command to safely close the program and free resources.
- **Completed by**: 
- **Assigned to**: Margaret Rivas

### Part 2: Navigation
- **Responsibilities**:
- [ ] Implement `cd [DIRNAME]` command to change the current working directory.
- [ ] Implement `ls` command to list directories in the current working directory.
- [ ] Error handling for non-existent directories or incorrect directory names.
- **Completed by**: 
- **Assigned to**:Margaret Rivas and Hannah Housand

### Part 3: Create
- **Responsibilities**:
- [ ] Implement `mkdir [DIRNAME]` command to create a new directory.
- [ ] Implement `creat [FILENAME]` command to create a new file with 0 bytes size.
- [ ] Handle errors for existing files or directories with the same name.
- **Completed by**: 
- **Assigned to**: Hannah Housand and Sophia Quinoa

### Part 4: Read
- **Responsibilities**:
- [ ]  Develop structure to store opened files.
- [ ] Implement `open [FILENAME] [FLAGS]` command to open files with specific modes.
- [ ] Implement `close [FILENAME]`, `lsof`, and `lseek [FILENAME] [OFFSET]` commands.
- [ ] Implement `read [FILENAME] [SIZE]` command to read data from a file.
- **Completed by**: 
- **Assigned to**: Margaret Rivas, Hannah Housand, Sophia Quinoa

### Part 5: Update
- **Responsibilities**:
- [ ] Implement `write [FILENAME] [STRING]` command to write data to a file.
- [ ] Implement `append [FILENAME] [STRING]` command to append data to a file.
- [ ] Error handling for non-existent files, directories, or incorrect file modes.
- **Completed by**: 
- **Assigned to**: Margaret Rivas

### Part 6: Delete
- **Responsibilities**:
- [ ] Implement `rm [FILENAME]` command to delete a file.
- [ ] Implement `rmdir [DIRNAME]` command to remove a directory.
- [ ] Error handling for non-existent files, directories, or when files are opened.
- **Completed by**: 
- **Assigned to**: Sophia Quinoa

### Extra Credit
- **Responsibilities**:
- [ ] Implement `rm -r [DIRNAME]` command for recursive deletion of directories and their contents.
- **Completed by**: 
- **Assigned to**: N/A

## File Listing
```
.
├── Makefile
├── README.md
├── fat32.img
├── include
│   ├── commands.h
│   ├── fat32.h
│   └── utils.h
└── src
    ├── commands.c
    ├── fat32.c
    ├── main.c
    └── utils.c
```
# How to Compile & Execute

### Requirements
- **Compiler**: `gcc` for C/C++
- **Dependencies**: <stdio.h>, <stdlib.h>, <string.h>, and <inttypes.h>

### Compilation
```bash
make
```
This will build the executable in root
### Execution
```bash
./filesys [FILENAME]

# Once inside executable run any of the available commands depicted above.
```

## Bugs
- **Bug 1**:
- **Bug 2**: 

## Considerations
- We assume FILENAME will be less than 11 characters (8 for name, 3 for extension).
- We assume No need to worry about “/” expansions into deeper directories.
- We assume file and directory names will not contain spaces or file extensions (e.g., .txt, .exe).
- We assume file and directory names will be names (not paths) within the current working directory.
- We expect to receive the path of the image file in the main program’s first argument (argv[1]).
