#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void do_cat(const char *path);
static void die(const char *s);
static void read_stdin();

int main(int argc, char *argv[]) {
  int i;
  if (argc < 2) {
    read_stdin();
  } else {
    for(i = 1; i < argc; i++) {
      do_cat(argv[i]);
    }
  }
  exit(0);
}

#define BUFFER_SIZE 2048

static void read_stdin() {
  int c;
  while((c = getchar() != EOF)) {
    printf("%d\n", c);
    if (putchar(c) < 0) exit(1);
  }
}

static void do_cat(const char *path) {
  int fd;
  unsigned char buf[BUFFER_SIZE];
  int n;

  fd = open(path, O_RDONLY);
  if(fd < 0) die(path);
  for (;;) {
    n = read(fd, buf, sizeof buf);
    printf("result = %d", n);
    if (n < 0) die(path);
    if (n == 0) break;
    if (write(STDOUT_FILENO, buf, n) < 0) die(path);
  }
  if (close(fd) < 0) die(path);
}

static void die(const char *s) {
  printf("message = %s\n", s);
  perror(s);
  exit(1);
}
