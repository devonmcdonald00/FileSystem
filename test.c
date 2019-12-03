#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "filesystem.h"
#include <stdbool.h>

int main(){    
    create_file("new file");
    create_file("another");
    return 0;
}