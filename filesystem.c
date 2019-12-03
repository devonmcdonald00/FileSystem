
// Implementation of filesystem.h

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include "softwaredisk.h"
#include "filesystem.h"
//#include "formatfs.c"


#define NUM_BLOCKS 5000
#define BLOCK_SIZE 512

typedef struct{
    unsigned char name[32];
    short inodeNumber;
}fileEntry;

typedef struct{
    short filesize;
    short indirectBlock; // 1 indirect block (will hold value of a block number)
    short directBlocks[12]; //12 direct blocks
}inode;

typedef struct FileInternals {
    char * name;
    short pos;
    FileMode mode; 
    bool open; //0 if closed, 1 if open
    short inodeNum;
} FileInternals;

FSError error = FS_NONE;

FileInternals * fileArray[75];

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

    //ABLE TO READ FROM SOFTWARE DISK CORRECTLY

    // check to see if filename already exists. FS_FILE_ALREADY_EXISTS
    // initialize directory table entry and link/create an inode
    // filename begins with NULL. FS_ILLEGAL_FILENAME

    int exists = file_exists(name);
    printf("\nANOTHER CREATION!!!!!!!!\n");

    if(exists == 0){
        //file doesn't exist so commence
        //data bitmap at block 6 and inode bitmap is at block 7
        int freeSpaceData = 0;
        int freeSpaceInode = 0;
        int blockCounter = 0;
        int dataBlockStart = 0;
        int inodeOffset = 0;
        bool * dataBitmap = malloc(512);
        short * inodeBitmap = malloc(512);

        bool * data = malloc(512);
        read_sd_block(dataBitmap, 6);
        
        while(freeSpaceData == 0){
            for(int i = dataBlockStart; i<7+dataBlockStart; i++){
                if(dataBitmap[i] == 0){
                    blockCounter++;
                }
            }
            if(blockCounter == 7){
                //7 bytes are free meaning 56 blocks are free for space
                freeSpaceData = 1;
            }
            else{
                //These 7 bytes aren't free so move to the next 7
                blockCounter = 0;
                dataBlockStart+=7;;
            }
        }
        printf("\nUpdating data bitmap... \n");
        for(int i = dataBlockStart; i<7+dataBlockStart; i++){
            //mark all bits of the byte as taken;
            // 11111111
            printf("\ndata reserved at %d\n", i);
            dataBitmap[i] = 1;
        }
        //dataBlockStart is now where the inode must link to 

        if(freeSpaceData == 0){
            //space is full
            error = FS_OUT_OF_SPACE;
            return NULL;
        }
        else{
            write_sd_block(dataBitmap, 6);
        }
        //Now find free inode
        read_sd_block(inodeBitmap, 7);
        for(int i = inodeOffset; i<75; i++){
            if(inodeBitmap[i] == 0){
                //bit in the ith position is low
                if(freeSpaceInode == 0){
                    inodeOffset = i;
                    //mark as taken in bitmap
                    inodeBitmap[i] = 1;
                    freeSpaceInode = 1;
                    printf("\nNew inode at %d\n", i);
                }
            }
        }
        
        if(inodeOffset == 75){
            error = FS_OUT_OF_SPACE;
            return NULL;
        }
        else{
            printf("\nUpdating inode bitmap... \n");
            write_sd_block(inodeBitmap, 7);

        }
        //now have inode offset and start block
        //Create inode
        inode newInode;
        newInode.filesize = 0;
        //metadata ends at block 13
        for(int i = 0; i<12; i++){
            newInode.directBlocks[i] = dataBlockStart+13+i;
        }
        printf("\nInode offset is %d and data block offset is %d\n", inodeOffset, dataBlockStart);
        
        newInode.indirectBlock = dataBlockStart+13+12;
        //bitmaps marked and inode created now update data
        //narrow down which block free inode is in
        inode * inputInodes = malloc((sizeof(inode)*15) + 92);
        if(inodeOffset < 15){
            read_sd_block(inputInodes, 8);
            inputInodes[inodeOffset] = newInode;
        }
        else if(inodeOffset < 30 && inodeOffset >= 15){
            read_sd_block(inputInodes, 9);
            inputInodes[inodeOffset-15] = newInode;
        }
        else if(inodeOffset < 45 && inodeOffset >= 30){
            read_sd_block(inputInodes, 10);
            inputInodes[inodeOffset-30] = newInode;
        }
        else if(inodeOffset < 60 && inodeOffset >= 45){
            read_sd_block(inputInodes, 11);
            inputInodes[inodeOffset-45] = newInode;
        }
        else if(inodeOffset < 75 && inodeOffset >= 60){
            read_sd_block(inputInodes, 12);
            inputInodes[inodeOffset-60] = newInode;
        }

        File newfile = malloc(sizeof(File));
        newfile->name = name;
        newfile->pos = 0;
        newfile->mode = 0; 
        newfile->open = 0;
        newfile->inodeNum = inodeOffset;

        free(dataBitmap);
        free(inodeBitmap);

        return newfile;
        
        //no data yet so no need to do anything with data blocks
    }
    else{
        error = FS_FILE_ALREADY_EXISTS;
        return NULL;
    }
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
    //Check first five blocks to see if the file name matches
    printf("right here");
    fileEntry * blockFileEntries = malloc((sizeof(fileEntry) * 15) + 2);
    int matching, temp = 0;
    int k;
    int entryNameLength = 0;
    for(int i = 1; i<=5; i++){
        read_sd_block(blockFileEntries, i);
        //check all 15 file entries in the ith block
        for(int j = 0; j < 15; j++){
            k = 0;
            while(blockFileEntries[j].name[k] != 0 || k < 32){
                if(name[k] == blockFileEntries[j].name[k]){
                    temp++;
                }
                k++;
            }
            if(temp > matching){
                //if the amount of matching characters is larger than all previous file entries then update matching
                matching = temp;
            }
            temp = 0;
        }
    }
    if(matching == 32){
        free(blockFileEntries);
        //if all 32 characters match then return file found
        return 1;
    }
    else{
        free(blockFileEntries);
        //if not all 32 characters match then file wasn't found
        return 0;
    }

}

// describe current filesystem error code by printing a descriptive message to standard
// error.
void fs_print_error(){
    switch (error) {
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
    case FS_FILE_ALREADY_EXISTS:
        printf("FS: Attempted creation of file with existing name.\n");
        break;
    case FS_EXCEEDS_MAX_FILE_SIZE:
        printf("FS: Seek or write would exceed max file size.\n");
        break;
    case FS_ILLEGAL_FILENAME:
        printf("FS: Filename begins with a null character.\n");
        break;
    case FS_IO_ERROR:
        printf("FS: Something really bad happened.\n");
        break;
    default:
        printf("SD: Unknown error code\n");
  }
}

// filesystem error code set (set by each filesystem function)