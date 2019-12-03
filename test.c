#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "filesystem.h"
#include <stdbool.h>

int main(){
    File newFile = malloc(sizeof(File));
    newFile = create_file("new file");
    //never opened so works correctly
    printf("\n file size is %ld!!!\n", file_length(newFile));    
    fs_print_error();
    return 0;
}