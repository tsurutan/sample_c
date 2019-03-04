#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main() {
  char buf[2048];
  int c;
  char *tmp_side = malloc(sizeof(char) * 100);
  int sum;

  if(fgets(buf, sizeof buf, stdin) == NULL) {
    exit(1);
  }
  sum = atoi(buf);

  if(fgets(buf, sizeof buf, stdin) == NULL) {
    exit(1);
  }

  for(int i = 0; i < sizeof buf; i++) {
    if(buf[i] == '\0') break;
    if(isspace(buf[i])) {
      sum += atoi(tmp_side);
      tmp_side[0] = '\0';
    } else {
      strcat(tmp_side, &buf[i]);
    }
  }

  if(fgets(buf, sizeof buf, stdin) == NULL) {
    exit(1);
  }

  printf("%d %s", sum, buf);
}
