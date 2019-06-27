#include <stdio.h>

#define BUFFER 512
int main(int argc, char **argv) {
  FILE *fp, *fin;
  char buf[BUFFER];
  int c;
  fp = fdopen(1, "w");
  fin = fdopen(0, "r");
  setbuf(stdin, NULL);
  setbuf(fp, NULL);
  setvbuf(fp, NULL, _IONBF, 0);
  while((c = fgetc(fin)) > 0) {
    putc(c, fp);
  }
}
