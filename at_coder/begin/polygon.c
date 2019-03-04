#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUF_SIZE 2048

static void calc(int n, int sides_count, int *sides);

int main() {
  char buf[BUF_SIZE];
  int n;
  int sides[100];
  char *tmp_side;
  int sides_count = 0;
  if(fgets(buf, sizeof buf, stdin)) {
    n = atoi(buf);
  }
  if(fgets(buf, sizeof buf, stdin) == NULL) {
    exit(1);
  }

  tmp_side = malloc(sizeof(char) * 100);
  for(int i = 0; i < sizeof buf; i++) {
    if(buf[i] == '\0') break;
    if(isspace(buf[i])) {
      int result = atoi(tmp_side);
      sides[sides_count] = result;
      tmp_side[0] = '\0';
      sides_count++;
    } else {
      strcat(tmp_side, &buf[i]);
    }
  }
  calc(n, sides_count, sides);
}

void calc(int n, int sides_count, int *sides) {
  int max_index = 0;
  int max = 0;
  int sum;
  if(n < 3 || n > 10) {
    printf("No");
    exit(1);
  }
  for(int i = 0; i < sizeof(sides); i++) {
    if(sides[i] > max) {
      max = sides[i];
      max_index = i;
    }
    if(sides[i] < 1 && sides[i] > 100) {
      printf("No");
      exit(1);
    }
  }
  for(int i = 0; i < sizeof(sides); i++) {
    if(i != max_index) {
      sum += sides[i];
    }
  }
  if(max >= sum) {
    printf("No");
  } else {
    printf("Yes");
  }
}
