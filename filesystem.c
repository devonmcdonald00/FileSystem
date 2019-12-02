
// Implementation of filesystem.h

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "softwaredisk.h"
#include "filesystem.h"
#include "formatfs.c"


#define NUM_BLOCKS 5000
#define BLOCK_SIZE 512


// open existing file with pathname 'name' and access mode 'mode'.  Current file
// position is set at byte 0.  Returns NULL on error. Always sets 'fserror' global.
File open_file(char *name, FileMode mode) {
    // open file name and return file name
    // should we have a bit for if the file is open or not?
    // test if file exists (go through FAT). FS_FILE_NOT_FOUND
    // test if file is already open. FS_FILE_OPEN
    // set current position to byte 0 by going to first block
    // traverse through the inode to get to first block
}


// create and open new file with pathname 'name' and (implied) access
// mode READ_WRITE. The current file position is set at byte 0.
// Returns NULL on error. Always sets 'fserror' global.
File create_file(char *name) {
    // check to see if filename already exists. FS_FILE_ALREADY_EXISTS
    // initialize directory table entry and link/create an inode
    // filename begins with NULL. FS_ILLEGAL_FILENAME
}


// close 'file'.  Always sets 'fserror' global.
void close_file(File file) {
    // see if file exists. FS_FILE_NOT_FOUND
    // see if file is open. FS_FILE_NOT_OPEN
    // set file attribute to closed
}

// read at most 'numbytes' of data from 'file' into 'buf', starting at the 
// current file position.  Returns the number of bytes read. If end of file is reached,
// then a return value less than 'numbytes' signals this condition. Always sets
// 'fserror' global.
unsigned long read_file(File file, void *buf, unsigned long numbytes){
    // see if file exists. FS_FILE_NOT_FOUND
    // see if file is open. FS_FILE_NOT_OPEN
    // look through file. iterate through blocks until numbytes is done
}

// write 'numbytes' of data from 'buf' into 'file' at the current file position. 
// Returns the number of bytes written. On an out of space error, the return value may be
// less than 'numbytes'.  Always sets 'fserror' global.
unsigned long write_file(File file, void *buf, unsigned long numbytes){
    // see if file exists. FS_FILE_NOT_FOUND
    // see if file is open. FS_FILE_NOT_OPEN
    // see if file is read only. FS_FILE_READ_ONLY
    // will write exceed max size. FS_EXCEEDS_MAX_FILE_SIZE
    // look through file and write at cuurent position. create new block if needed.
}

// sets current position in file to 'bytepos', always relative to the
// beginning of file.  Seeks past the current end of file should
// extend the file. Returns 1 on success and 0 on failure.  Always
// sets 'fserror' global.
int seek_file(File file, unsigned long bytepos){
    // see if file exists. FS_FILE_NOT_FOUND
    // see if file is open. FS_FILE_NOT_OPEN
    // will seek exceed max size. FS_EXCEEDS_MAX_FILE_SIZE
    // set current position to bytepos
}

// returns the current length of the file in bytes. Always sets 'fserror' global.
unsigned long file_length(File file){
    // see if file exists. FS_FILE_NOT_FOUND
    // see if file is open. FS_FILE_NOT_OPEN
    // traverse through file with a counter
}

// deletes the file named 'name', if it exists. Returns 1 on success, 0 on failure. 
// Always sets 'fserror' global.   
int delete_file(char *name){
    // see if file exists. FS_FILE_NOT_FOUND
    // see if file isopen. FS_FILE_OPEN
    // free up all blocks of data connected to that file
    // remove the directory entry. free inodes too.
}

// determines if a file with 'name' exists and returns 1 if it exists, otherwise 0.
// Always sets 'fserror' global.
int file_exists(char *name){
    // see if file exists. FS_FILE_NOT_FOUND
    // check table entries to see if the name matches. if so, return 1.
}

// describe current filesystem error code by printing a descriptive message to standard
// error.
void fs_print_error(void){
    switch (fserror) {
    case FS_NONE:
        printf("FS: No error.\n");
        break;
    case FS_OUT_OF_SPACE:
        printf("FS: The operation caused the software disk to fill up.\n");
        break;
    case FS_FILE_NOT_OPEN:
        printf("FS: Attempted read/write/close/etc. on file that isn’t open.\n");
        break;
    case FS_FILE_OPEN:
        printf("FS: File is already open.\n");
        break;
    case FS_FILE_NOT_FOUND:
        printf("FS: Attempted open or delete of file that doesn’t exist.\n");
        break;
    case FS_FILE_READ_ONLY:
        printf("FS: Attempted write to file opened for READ_ONLY.\n");
        break;
    case: FS_FILE_ALREADY_EXISTS:
        printf("FS: Attempted creation of file with existing name.\n")
        break;
    case: FS_EXCEEDS_MAX_FILE_SIZE:
        printf("FS: Seek or write would exceed max file size.\n")
        break;
    case FS_ILLEGAL_FILENAME:
        printf("FS: Filename begins with a null character.\n");
        break;
    case FS_IO_ERROR:
        printf("FS: Something really bad happened.\n")
        break;
    default:
        printf("SD: Unknown error code %d.\n", sderror);
  }
}

// filesystem error code set (set by each filesystem function)
extern FSError fserror;