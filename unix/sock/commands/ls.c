#include <stdio.h>
#include <dirent.h>
#include "ls.h"

int ls(char *path) {
  DIR *dir;
  struct dirent *dent;
  dir = opendir(path);
  while((dent = readdir(dir)) != NULL) {
    printf("%llu %hu %hhu %s\n", dent->d_ino, dent->d_reclen, dent->d_type, dent->d_name);
  }
  closedir(dir);
  return 0;
}
