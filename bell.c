#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  printf("stdin_fileno=%d", STDIN_FILENO);
  printf("\007");
  exit(0);
}
