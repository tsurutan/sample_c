#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER 514

int main(int argi, char **argc) {
  int fd, ls;
  char buf[BUFFER];
  char tmp_output[BUFFER];
  tmp_output[0] = 't';
  fd = open("./output/3_6.txt", O_RDWR | O_APPEND);
  ls = lseek(fd, 20, SEEK_SET);
  write(fd, tmp_output, 1);
  printf("ls value = %d", ls);
}
