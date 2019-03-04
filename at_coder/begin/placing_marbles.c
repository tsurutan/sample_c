#include <stdio.h>

int main() {
  char buf[3];
  int sum = 0;
  scanf("%s", buf);
  for(int i = 0; i < sizeof(buf); i++) {
    if(buf[i] == '1') {
      sum += 1;
    }
  }
  printf("%d", sum);
}
