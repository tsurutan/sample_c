#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  if(S_IFLNK(41453)) {
    printf("symbolic link!!");
  }
}
