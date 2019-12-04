#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

// RUN formatfs before conducting this test!


int main(int argc, char *argv[]) {
  int ret;
  File f;
  char buf[1000];

  // should succeed
  f=create_file("simple");
  printf("ret from create_file(\"blarg\") = %p\n",
	 f);
  fs_print_error();

  // should succeed
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
  