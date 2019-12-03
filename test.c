#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "filesystem.h"
#include <stdbool.h>

int main(){
    File newFile = malloc(sizeof(File));
    newFile = create_file("new file");
    char buf[300];
    int ret = 12;
    open_file("new file", READ_WRITE);
    ret = write_file(newFile, "Hello file!!", strlen("Hello file!!"));
    fs_print_error();
    printf("\nret was %d\n", ret);
    ret = read_file(newFile, buf, strlen("Hello file!!"));
    printf("\nThe buffer is %s\n", buf);
    fs_print_error();
    printf("\nret was %d for read\n", ret);
    //never opened so works correctly
    //printf("\n file size is %ld!!!\n", file_length(newFile));    
    return 0;
}