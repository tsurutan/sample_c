#include <stdio.h>

void sort(int n, int *a);
int main() {
  int n, a[200], i, alice = 0, bob = 0;
  scanf("%d", &n);
  for(i = 0; i < n; i++) scanf("%d", &a[i]);
  sort(n, a);
  for(i = 0; i < n; i++) {
    if(i % 2 == 0) {
      alice += a[i];
    } else {
      bob += a[i];
    }
  }
  printf("%d", alice - bob);
}

void sort(int n, int *a) {
  for(int i = 0; i < n; i++) {
    int max = 0;
    for(int j = i; j < n; j++) {
      if(a[j] > max) {
        max = a[j];
        a[j] = a[i];
        a[i] = max;
      }
    }
  }
}
