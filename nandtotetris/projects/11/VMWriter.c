#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "VMWriter.h"

#define BUFFER 512

static FILE *fp_output;
char class_name[BUFFER] = { '\0' };

void VMWriter__constructor(char *name) {
  fp_output = fopen(name, "w+");
}

void VMWriter__close(void) {
  fclose(fp_output);
}

void write_push(Segment segment, char *index) {
  char output[BUFFER] = { '\0' };
  switch(segment) {
    case S_CONST:
      sprintf(output, "push constant %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_ARG:
      sprintf(output, "push argument %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_LOCAL:
      sprintf(output, "push local %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_STATIC:
      sprintf(output, "push static %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_THIS:
      sprintf(output, "push this %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_THAT:
      sprintf(output, "push that %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_POINTER:
      sprintf(output, "push pointer %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_TEMP:
      sprintf(output, "push temp %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
  }
}

void write_pop(Segment segment, char *index) {
  char output[BUFFER] = { '\0' };
  switch(segment) {
    case S_CONST:
      sprintf(output, "pop constant %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_ARG:
      sprintf(output, "pop argument %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_LOCAL:
      sprintf(output, "pop local %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_STATIC:
      sprintf(output, "pop static %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_THIS:
      sprintf(output, "pop this %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_THAT:
      sprintf(output, "pop that %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_POINTER:
      sprintf(output, "pop pointer %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
    case S_TEMP:
      sprintf(output, "pop temp %s\n", index);
      fwrite(output, strlen(output), 1, fp_output);
      break;
  }
}

void write_return() {
  fwrite("return\n", strlen("return\n"), 1, fp_output);
}

void write_call(char *name, int n_locals) {
  char function_name[BUFFER] = { '\0' };
  sprintf(function_name, "call %s %d\n", name, n_locals);
  fwrite(function_name, strlen(function_name), 1, fp_output);
}

void write_arithmetic(char *op) {
  if (strstr(op, "+") != NULL) {
    printf("add \n");
    fwrite("add\n", strlen("add\n"), 1, fp_output);
  } else if (strstr(op, "*") != NULL) {
    printf("mult \n");
    fwrite("call Math.multiply 2\n", strlen("call Math.multiply 2\n"), 1, fp_output);
  }
}

void write_function(char *name, int n_locals) {
  char function_name[BUFFER] = { '\0' };
  sprintf(function_name, "function %s.%s %d\n", class_name, name, n_locals);
  fwrite(function_name, strlen(function_name), 1, fp_output);
}
