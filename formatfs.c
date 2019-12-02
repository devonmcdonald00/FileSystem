#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "softwaredisk.h"

#define NUM_BLOCKS 5000
#define BLOCK_SIZE 512

//Need to create the metadata first

typedef struct{
    short filesize;
    short indirectBlock; //1 indirect block (will hold value of a block number)
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


inode * setupInodes(int last){
    //inode type is 28 bytes
    if(last == 0){
        inode * inodeEntries = malloc(sizeof(inode) * 18);
        inode temp;
        temp.filesize = 0;
        temp.indirectBlock = 0;
        for(int i = 0; i<12; i++){
            temp.directBlocks[i] = 0;
        }
        for(int i = 0; i<18; i++){
            inodeEntries[i] = temp;
        }
        return inodeEntries;
    }
    else{
        //The last block of inodes will only have 3 inodes to make 75 total inodes
        inode * inodeEntries = malloc(sizeof(inode) * 3);
        inode temp;
        temp.filesize = 0;
        temp.indirectBlock = 0;
        for(int i = 0; i<3; i++){
            temp.directBlocks[i] = 0;
        }
        for(int i = 0; i<3; i++){
            inodeEntries[i] = temp;
        }
        return inodeEntries;
    }
    

}

fileEntry * setupDirectoryEntries(){
    //fileEntry type is 34 bytes
    fileEntry * directoryEntries = malloc(sizeof(fileEntry) * 15);
    fileEntry temp;
    for(int i = 0; i<32 ; i++){
        temp.name[i] = 65;
    }
    temp.inodeNumber = 0;
    for(int i = 0; i < 15; i++){
        directoryEntries[i] = temp;
    }

    return directoryEntries;
}

int main(){

    int init = init_software_disk();

    if(init == 1){
        //Init worked define metadata
        printf("Initialization of the disk worked\n");

        //Define inode and data bitmaps (6th and 7th blocks respectively)
        unsigned char * inodeBitmap;
        unsigned char * dataBitmap;
        inodeBitmap = setupInodeBitmap();
        dataBitmap = setupDataBitmap();

        unsigned char * testData = malloc(sizeof(char) * 512);
        write_sd_block(dataBitmap, 6);
        write_sd_block(inodeBitmap, 7);

        //Define Directory Entry with file entries (first 5 blocks)
        fileEntry * inputDirectoryEntries = malloc((sizeof(fileEntry) * 15) + 2);
        inputDirectoryEntries = setupDirectoryEntries();
        write_sd_block(inputDirectoryEntries, 1);
        write_sd_block(inputDirectoryEntries, 2);
        write_sd_block(inputDirectoryEntries, 3);
        write_sd_block(inputDirectoryEntries, 4);
        write_sd_block(inputDirectoryEntries, 5);
        fileEntry * testDirectoryRead = malloc((sizeof(fileEntry)*15)+2);
        read_sd_block(testDirectoryRead, 1);

        printf("\nThe first character of name is %c and should be A\n", testDirectoryRead[0].name[0]);

        //Define inode blocks
        inode * inputInodes = malloc((sizeof(inode)*18) + 8);
        inode * lastInodes = malloc((sizeof(inode)*3) + 428);

        inputInodes = setupInodes(0);
        lastInodes = setupInodes(1);

        write_sd_block(inputInodes, 8);
        write_sd_block(inputInodes, 9);
        write_sd_block(inputInodes, 10);
        write_sd_block(inputInodes, 11);
        write_sd_block(lastInodes, 12);

        //All metadata finished

    }
    else{
        //There was an error
        printf("Error with init");
    }

    return 0;
}