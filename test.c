#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "filesystem.h"

int main(){
    int found;
    found = file_exists("AAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    printf("\nThe output of file_exists is %d", found);
    return 0;
}