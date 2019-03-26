#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "JackTokenizer.h"
#define BUFFER 1024

void set_filename(char *path, char *output);

int main(int argc, char **argv) {
  FILE *fp_input = NULL;
  FILE *fp_output = NULL;
  char filename[BUFFER];

  set_filename(argv[1], filename);
  printf("filename = %s\n", filename);
  fp_input = fopen(argv[1], "r");
  fp_output = fopen("result.vm", "w+");
  JackTokenizer_constructor(fp_input, fp_output);
}

void set_filename(char *path, char *output) {
  int offset = 0, path_len = strlen(path);
  for(int i = path_len; i > 0; i--) {
    if(path[i] == '/') break;
    offset++;
  }
  strncpy(output, path + path_len - offset + 1, path_len - 4);
}
