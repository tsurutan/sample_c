#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int printf_ret = -1;
  printf_ret = printf("");
  printf("printf ret = %d", printf_ret);
  exit(0);
}
