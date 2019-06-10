#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER 11

int main(int argc, char **argv) {
  int fd;
  umask(0);
  char tmp[BUFFER] = "0123456789";
  fd = open(argv[1], O_CREAT | O_WRONLY);
  printf("tmp = %s\n", tmp);
  write(fd, tmp, BUFFER);
  lseek(fd, BUFFER, SEEK_CUR);
  write(fd, tmp, BUFFER);
  close(fd);
}
