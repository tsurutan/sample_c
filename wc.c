#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void do_cat(const char *path);
static void die(const char *s);

int main(int argc, char *argv[]) {
  int i;
  if (argc < 2) {
    fprintf(stderr, "%s: file name not given\n", argv[0]);
    exit(1);
  }
  for(i = 1; i < argc; i++) {
    do_cat(argv[i]);
  }
  exit(0);
}

#define BUFFER_SIZE 2048

static void do_cat(const char *path) {
  int fd;
  unsigned char buf[BUFFER_SIZE];
  int n;

  fd = open(path, O_RDONLY);
  int nl_count = 0;
  for (;;) {
    n = read(fd, buf, sizeof buf);
    if (n < 0) die(path);
    if (n == 0) break;
    for(int i = 0; i < n; i++) {
      if (buf[i] == '\n') nl_count++;
    }
  }
  printf("new line count = %d", nl_count);
  if (close(fd) < 0) die(path);
}

static void die(const char *s) {
  perror(s);
  exit(1);
}
