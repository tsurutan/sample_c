#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *test = "test";
  printf("test = %s\n", test);
  printf("pointer test = %c\n", *test);
  printf("pointer address = %s\n", (*test));
  test += 1;
  printf("pointer test = %c\n", *test);
}
