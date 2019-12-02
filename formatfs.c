#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include "softwaredisk.h"

#define NUM_BLOCKS 5000
#define BLOCK_SIZE 512

//Need to create the metadata first

typedef struct{
    short filesize;
    short indirectBlock; // 1 indirect block (will hold value of a block number)
    short directBlocks[12]; //12 direct blocks
}inode;


typedef struct{
    unsigned char name[32];
    short inodeNumber;
}fileEntry;


unsigned char * setupDataBitmap(){
    unsigned char *dataBitmap = malloc(sizeof(char) * BLOCK_SIZE);
    //75 inodes requires 80 bits so need 10 bytes
    for(int i = 0; i < BLOCK_SIZE; i++){
        //initialize to all zeros
        dataBitmap[i] = 0;
    }

    //For testing uncomment this -> should set each byte to a capital A
    // for(int i = 0; i < 10; i++){
    //     inodeBitmap[i] |= 0;s
    //     inodeBitmap[i] |= (1 << 6);
    //     inodeBitmap[i] |= (1 << 0);
    // }
    //printf("%c", inodeBitmap[9]);
    return dataBitmap;
}

unsigned char * setupInodeBitmap(){
    unsigned char *inodeBitmap = malloc(sizeof(char) * BLOCK_SIZE);
    //75 inodes requires 80 bits so need 10 bytes
    //However there we have to go all the way up to the size of the block
    for(int i = 0; i < BLOCK_SIZE; i++){
        //initialize to all zeros
        inodeBitmap[i] = 0;
    }

    //For testing uncomment this -> should set each byte to a capital A
    // for(int i = 0; i < 10; i++){
    //     inodeBitmap[i] = 65;
    // }
    //printf("%c", inodeBitmap[9]);
    return inodeBitmap;
}


inode * setupInodes(){
    //inode type is 28 bytes
    inode * inodeEntries = malloc(sizeof(inode) * 15);
    inode temp;
    temp.filesize = 0; // file is empty initially (2 bytes)
    temp.indirectBlock = 0; // no indirect block intially (2 bytes)
    for(int i = 0; i<12; i++){
        temp.directBlocks[i] = 0; // initialize 12 direct blocks to 0 (24 bytes)
    }
    for(int i = 0; i<15; i++){
        inodeEntries[i] = temp; // 15 inodes per block
    }
    return inodeEntries;
}

fileEntry * setupDirectoryEntries(){
    //fileEntry type is 34 bytes
    fileEntry * directoryEntries = malloc(sizeof(fileEntry) * 15);
    fileEntry temp;
    for(int i = 0; i<32 ; i++){
        temp.name[i] = 0; // initialize 32 bytes for file name. NULL
    }
    temp.inodeNumber = 0; // initialize to 0 (2 bytes)
    for(int i = 0; i < 15; i++){
        directoryEntries[i] = temp; // 15 file entries per block
    }

    return directoryEntries;
}

int main(){

    int init = init_software_disk();

    if(init == 1){
        //Init worked define metadata
        printf("Disk has been initialized...\n");

        //Define inode and data bitmaps (6th and 7th blocks respectively)
        unsigned char * inodeBitmap;
        unsigned char * dataBitmap;
        inodeBitmap = setupInodeBitmap();
        dataBitmap = setupDataBitmap();
        printf("\nBitmaps setup...\n");

        unsigned char * testData = malloc(sizeof(char) * 512);
        write_sd_block(dataBitmap, 6); // block for data bitmap
        write_sd_block(inodeBitmap, 7); // block for inode bitmap

        //Define Directory Entry with file entries (first 5 blocks)
        fileEntry * inputDirectoryEntries = malloc((sizeof(fileEntry) * 15) + 2);
        inputDirectoryEntries = setupDirectoryEntries();
        write_sd_block(inputDirectoryEntries, 1); // 1 of 5 FAT blocks
        write_sd_block(inputDirectoryEntries, 2); // 1 of 5 FAT blocks
        write_sd_block(inputDirectoryEntries, 3); // 1 of 5 FAT blocks
        write_sd_block(inputDirectoryEntries, 4); // 1 of 5 FAT blocks
        write_sd_block(inputDirectoryEntries, 5); // 1 of 5 FAT blocks
        fileEntry * testDirectoryRead = malloc((sizeof(fileEntry)*15)+2);
        read_sd_block(testDirectoryRead, 1);

        printf("\nDirectory entries have been setup...\n");

        //Define inode blocks
        inode * inputInodes = malloc((sizeof(inode)*15) + 92);

        inputInodes = setupInodes();

        write_sd_block(inputInodes, 8); // 1 of 5 inode blocks
        write_sd_block(inputInodes, 9); // 1 of 5 inode blocks
        write_sd_block(inputInodes, 10); // 1 of 5 inode blocks
        write_sd_block(inputInodes, 11); // 1 of 5 inode blocks
        write_sd_block(inputInodes, 12); // 1 of 5 inode blocks

        printf("\n75 inodes have been setup...\n\nALL METADATA HAS BEEN SETUP\n");
        //All metadata finished

    }
    else{
        //There was an error
        //printf("Error with init");
        sd_print_error();
    }

    return 0;
}