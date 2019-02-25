#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

static void do_ls(char *path);
void error(char *s) {
  perror(s);
  exit(1);
}


int main(int argc, char *argv[]) {
  int i;
  FILE *f;
  if (argc < 2) {
    exit(1);
  }
  for (i = 1; i < argc; i++) {
    f = fopen(argv[i], "r");
    unlink(argv[i]);
    int c;
    while((c = fgetc(f)) != EOF) {
      putchar(c);
    }
    fclose(f);
  }
}
