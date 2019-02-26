#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#define BUF 256
int
main (int argc, char *argv[])
{
  char *test = (char*) malloc(sizeof(char) * 200);
  test = "testestste";
  puts(test);
}
