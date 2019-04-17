#include <stdio.h>
#include <dirent.h>
#include <string.h>

FILE *fp_output;
void VMWriter__constructor(char *name) {
  fp_output = fopen(name, "w+");
}

void write_function(char *name, int n_locals) {
  fwrite(name, strlen(name), 1, fp_output);
}
