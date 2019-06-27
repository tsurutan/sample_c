#include <stdio.h>
#include <stdlib.h>
#define BUFFER 512

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  char buf[BUFFER];

  if(argc < 3) {
    printf("argc is less");
    exit(0);
  }
  fpin = fopen(argv[1], "r");
  fpout = fopen(argv[2], "w");
  while(fgets(buf, BUFFER, fpin)) {
    fputs(buf, fpout);
  }
}
