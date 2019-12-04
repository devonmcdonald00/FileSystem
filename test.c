#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

// RUN formatfs before conducting this test!
  
int main(int argc, char *argv[]) {
  int ret, i;
  File f;
  char buf[1024], buf2[1024];

  // tests block alignment, along with testfs4a
  
  f=open_file("line-em-up", READ_WRITE);
  printf("ret from open_file(\"line-em-up\", READ_WRITE) = %p\n",
	 f);
  fs_print_error();
  
  for (i=0; i < 50; i+=2) {
    memset(buf,     'A'+i, 512);
    memset(buf+512, 'A'+i+1, 512);
    ret = read_file(f, buf2, 1024);
    printf("ret from read_file(f, buf2, 1024) = %d\n",
	   ret);
    fs_print_error();
    printf("Expected buffer value %s.\n",
	 ! memcmp(buf, buf2, 1024) ? "matches" : "doesn't  match");
  }

  close_file(f);
  printf("Executed close_file(f).\n");
  fs_print_error();
}