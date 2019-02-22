#include <stdio.h>
#include <stdlib.h>

void error(char *s) {
  perror(s);
  exit(1);
}

int main(int argc, char *argv[]) {
  int i;
  for (i = 1; i < argc; i++) {
    FILE *f;
    int c;

    f = fopen(argv[i], "r");
    if (!f) {
      error(argv[i]);
    }
    while((c = fgetc(f)) != EOF) {
      if (putchar(c) < 0) error(NULL);
    }
    fclose(f);
  }
  exit(0);
}
