#include <stdio.h>
#include <stdlib.h>

void error(char *s) {
  perror(s);
  exit(1);
}

void output(FILE *f) {
  int c;
  while((c = fgetc(f)) != EOF) {
    if (putchar(c) < 0) error(NULL);
  }
  fclose(f);
}

int main(int argc, char *argv[]) {
  int i;
  if (argc < 2) {
    output(stdin);
  } else {
    for (i = 1; i < argc; i++) {
      FILE *f;

      f = fopen(argv[i], "r");
      if (!f) {
        error(argv[i]);
      }
      output(f);
    }
  }
  exit(0);
}
