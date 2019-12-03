#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "filesystem.h"
#include <stdbool.h>

int main(){
    File newFile = malloc(sizeof(File));
    newFile = create_file("new file");
    printf("\nThe file exists status %d\n", file_exists("new file"));
    fs_print_error();
    printf("\nWas deleted equals %d\n", delete_file("new file"));
    fs_print_error();
    printf("\nThe file exists status %d\n", file_exists("new file"));
    //never opened so works correctly
    //printf("\n file size is %ld!!!\n", file_length(newFile));    
    fs_print_error();
    return 0;
}