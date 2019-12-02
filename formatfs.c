#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "softwaredisk.h"

#define NUM_BLOCKS 5000
#define BLOCK_SIZE 512

//Need to create the metadata first

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


int main(){

    int init = init_software_disk();

    if(init == 1){
        //Init worked define metadata
        printf("init worked\n");

        //Define inode and data bitmaps
        unsigned char * inodeBitmap;
        unsigned char * dataBitmap;
        inodeBitmap = setupInodeBitmap();
        dataBitmap = setupDataBitmap();

        unsigned char * testData = malloc(sizeof(char) * 512);
        write_sd_block(dataBitmap, 6);
        write_sd_block(inodeBitmap, 7);

        read_sd_block(testData, 7);
        printf("%c", testData[0]);

        //Bitmaps setup correctly

    }
    else{
        //There was an error
        printf("Error with init");
    }

    return 0;
}