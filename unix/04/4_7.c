#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER 10

int main(int argc, char **argv) {
  int fd1, fd2, size;
  char buf[BUFFER];
  fd1 = open(argv[1], O_RDONLY);
  umask(0);
  fd2 = open(argv[2], O_CREAT | O_RDWR);

  while((size = read(fd1, buf, BUFFER)) > 0) {
    write(fd2, buf, size);
    if(size < BUFFER) { break; }
  }
  close(fd2);
  close(fd1);
}
