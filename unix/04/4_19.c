#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
  int fd;
  unlink("/dev/fd/1");
  if((fd = creat("/dev/fd/1", FILESEC_MODE)) < 0) {
    printf("error");
  }
}
