#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

// RUN formatfs before conducting this test!


int main(int argc, char *argv[]) {
  int ret;
  File f;
  char buf[1000];

  // should fail, file doesn't exist
  printf("ret from delete_file(\"blarg\") = %d\n",
	 delete_file("blarg"));
  fs_print_error();

  // should fail, file doesn't exist
  printf("ret from open_file(\"blarg\", READ_ONLY) = %p\n",
	 open_file("blarg", READ_ONLY));
  fs_print_error();

  // should succeed
  f=create_file("blarg");
  printf("ret from create_file(\"blarg\") = %p\n",
	 f);
  fs_print_error();

  // should succeed
  close_file(f);
  printf("Executed close_file(f).\n");
  fs_print_error();

  // should fail, file now exists
  f=create_file("blarg");
  printf("ret from create_file(\"blarg\") = %p\n",
	 f);
  fs_print_error();
  
  // should fail, file not open
  close_file(f);
  printf("Executed close_file(f).\n");
  fs_print_error();
  
  // should succeed
  f=open_file("blarg", READ_ONLY);
  printf("ret from open_file(\"blarg\", READ_ONLY) = %p\n",
	 f);
  fs_print_error();
  
  // should fail, file is open for read-only
  ret=write_file(f, "hello", strlen("hello"));
  printf("ret from write_file(f, \"hello\", strlen(\"hello\") = %d\n",
	 ret);
  fs_print_error();

  // should succeed
  close_file(f);
  printf("Executed close_file(f).\n");
  fs_print_error();

  // should succeed
  f=open_file("blarg", READ_WRITE);
  printf("ret from open_file(\"blarg\", READ_WRITE) = %p\n",
	 f);
  fs_print_error();
  
  // should succeed, file is now open for read-write
  ret=write_file(f, "hello", strlen("hello"));
  printf("ret from write_file(f, \"hello\", strlen(\"hello\") = %d\n",
	 ret);
  fs_print_error();
 
  // should succeed
  printf("Seeking to beginning of file.\n");
  seek_file(f, 0);
  fs_print_error();

  // should succeed
  bzero(buf, 1000);
  ret=read_file(f, buf, strlen("hello"));
  printf("ret from read_file(f, buf, strlen(\"hello\") = %d\n",
	 ret);
  printf("buf=\"%s\"\n", buf);
  fs_print_error();

  // should succeed
  close_file(f);
  printf("Executed close_file(f).\n");
  fs_print_error();
}