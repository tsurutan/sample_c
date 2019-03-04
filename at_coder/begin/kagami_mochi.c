#include <stdio.h>

int main() {
  int n, a[200], i, alice = 0, bob = 0, count = 0, dup = 0;
  scanf("%d", &n);
  for(i = 0; i < n; i++) {
    scanf("%d", &a[i]);
    dup = 0;
    for(int j = 0; j < i; j++) {
      if(a[i] == a[j]) {
        dup = 1;
        break;
      }
    }
    if(!dup) count++;
  }
  printf("%d", count);
}
