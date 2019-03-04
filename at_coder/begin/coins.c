#include <stdio.h>

int main() {
  int a, b, c, x, count = 0;
  scanf("%d", &a);
  scanf("%d", &b);
  scanf("%d", &c);
  scanf("%d", &x);
  if(x % 50 != 0) {
    printf("0");
  } else {
    for(int i_a = 0; i_a <= a; i_a++) {
      for(int i_b = 0; i_b <= b; i_b++) {
        for(int i_c = 0; i_c <= c; i_c++) {
          if (i_a * 500 + i_b * 100 + i_c * 50 == x) count++;
        }
      }
    }
    printf("%d", count);
  }
}
