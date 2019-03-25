#include <dirent.h>
#include <stdio.h>
#include <string.h>

int end_with(char *pre, char *target);
void parse_filename(char *path, char *output);

int main (int argc, char **argv) {
  char *path;
  struct dirent *dent;
  DIR *dir;
  path = argv[1];
  dir = opendir(path);
  parse_filename(path, filename);
  printf("filename = %s\n", filename);
  while ((dent = readdir(dir)) != NULL) {
    if(end_with("vm", dent->d_name)) {
      printf("%s\n", dent->d_name);
    }
  }
  closedir(dir);
}

int end_with(char *pre, char *target) {
  size_t pre_size = strlen(pre), target_size = strlen(target);
  /* printf("starts_with: pre_size = %zu, target_size = %zu\n", pre_size, target_size); */
  /* printf("starts_with: pre = %s, target = %s\n", pre, target); */
  if(pre_size > target_size) return 0;
  /* printf("starts_with: result = %s\n", target + target_size - pre_size); */
  int result = strncmp(pre, target + target_size - pre_size, pre_size) == 0 ? 1 : 0;
  /* printf("starts_with: result = %d\n", result); */
  return result;
}
