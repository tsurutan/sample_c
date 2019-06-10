#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 514

int main(int argi, char **argc) {
  int fd;
  printf("fd\n");
  while(1) {
    int n;
    char buf[BUFFER_SIZE];
    if((n = read(0, buf, BUFFER_SIZE)) == -1) {
      exit(1);
    }
    fd = dup(0);
    dprintf(fd, "fd = %d", fd);
    write(1, buf, n);
  }
}
