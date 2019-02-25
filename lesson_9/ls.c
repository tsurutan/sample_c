#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

static void do_ls(char *path);

int main(int argc, char *argv[]) {
  int i;
  if (argc < 2) {
    exit(1);
  }
  for (i = 1; i < argc; i++) {
    do_ls(argv[i]);
  }
}

static void do_ls(char *path) {
  DIR *d;
  struct dirent *ent;

  d = opendir(path);
  if(!d) {
    perror(path);
    exit(1);
  }

  while((ent = readdir(d))) {
    if(ent->d_type == DT_DIR) {
      if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
      char c_path[1024];
      sprintf(c_path, "%s/%s", path, ent->d_name);
      printf("%s---------\n", c_path);
      do_ls(c_path);
    } else {
      printf("%s\n", ent->d_name);
    }
  }
  closedir(d);
}
