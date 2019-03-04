#include <stdio.h>

int sum_digit(int num);
int main() {
  int a, b, n, sum, tmp_sum, count = 0;
  scanf("%d %d %d", &n, &a, &b);
  for(int i = 1; i <= n; i++) {
    tmp_sum = sum_digit(i);
    if(tmp_sum >= a && tmp_sum <=b ) sum += i;
  }
  printf("%d", sum);
}

int sum_digit(int num) {
  int sum = 0;
  for(int i = 10000; i >= 1; i /= 10) {
    sum += (num / i);
    num %= i;
  }
  return sum;
}
