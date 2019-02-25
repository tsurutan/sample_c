#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *first = "first";
  char *second = "second";
  char output[1024];
  sprintf(output, "%s%s\n", first, second);

  printf("%s", output);
}
