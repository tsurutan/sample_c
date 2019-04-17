#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "JackTokenizer.h"
#include "CompilationEngine.h"
#include "SymbolTable.h"
#include "VMWriter.h"
#define BUFFER 1024

void set_filename(char *path, char *output, char *suffix);

int main(int argc, char **argv) {
  FILE *fp_input = NULL;
  FILE *fp_output = NULL;
  FILE *fp_compile_input = NULL;
  FILE *fp_compile_output = NULL;
  char filename[BUFFER] = {'\0'};

  set_filename(argv[1], filename, "Token.xml");
  fp_input = fopen(argv[1], "r");
  fp_output = fopen(filename, "w+");
  JackTokenizer_constructor(fp_input, fp_output);
  fclose(fp_input);
  fclose(fp_output);

  fp_compile_input = fopen(filename, "r");
  set_filename(argv[1], filename, "Compiled.xml");
  fp_compile_output = fopen(filename, "w+");
  set_filename(argv[1], filename, ".vm");
  VMWriter__constructor(filename);
  CompilationEngine_constructor(fp_compile_input, fp_compile_output);
  fclose(fp_compile_input);
  fclose(fp_compile_output);
}

void set_filename(char *path, char *output, char *suffix) {
  int offset = 0, path_len = strlen(path);
  char filename[BUFFER] = {'\0'};
  char path_name[BUFFER] = {'\0'};
  for(int i = path_len; i > 0; i--) {
    if(path[i] == '/') break;
    offset++;
  }
  strncpy(filename, path + path_len - offset + 1, offset - 6);
  strncpy(path_name, path, path_len - offset);
  sprintf(output, "%s/%s%s", path_name, filename, suffix);
}
