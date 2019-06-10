#include <stdio.h>
#include <dirent.h>

int main(int argc, char **argv) {
  DIR *dir;
  struct dirent *dent;
  dir = opendir(argv[1]);
  while((dent = readdir(dir)) != NULL) {
    printf("name = %s\n", dent->d_name);
  }
}
