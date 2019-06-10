#include <unistd.h>
#include <string.h>

#define BUFFER 512

int main(int argi, char **argc) {
  int new_fd = dup2(0, 14);
  write(14, argc[1], strlen(argc[1]));
}
