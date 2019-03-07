#include <stdio.h>
#include <unistd.h>

int main() {
  if(daemon(0, 0) < 0) {
    printf("failure");
  } else {
    printf("success");
  }
}
