#include <stdio.h>

int main() {
  int n, a[200], flag = 1, count = -1, i;
  scanf("%d", &n);
  for(i = 0; i < n; i++) scanf("%d", &a[i]);
  do {
    count++;
    for(i = 0; i < n; i++) {
      if(a[i] % 2 != 0) { flag = 0; break; }
      a[i] /= 2;
    }
  } while(flag);
  printf("%d", count);
}
