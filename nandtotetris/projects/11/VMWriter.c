#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "VMWriter.h"
#include "SymbolTable.h"

#define BUFFER 512

static FILE *fp_output;
char class_name[BUFFER] = { '\0' };

void VMWriter__constructor(char *name) {
  fp_output = fopen(name, "w+");
}

void VMWriter__close(void) {
  fclose(fp_output);
}

void write_string(char *value) {
  char output_arg[BUFFER];
  sprintf(output_arg, "%d", strlen(value));
  write_push(S_CONST, output_arg);
  fwrite("call String.new 1\n", strlen("call String.new 1\n"), 1, fp_output);
  for(int i = 0; i < strlen(value); i++) {
    char val = value[i];
    char output[BUFFER];
    sprintf(output, "%d", val);
    write_push(S_CONST, output);
  fwrite("call String.appendChar 2\n", strlen("call String.appendChar 2\n"), 1, fp_output);
  }
}

void write_constructor(int class_index) {
  char output[BUFFER];
  sprintf(output, "push constant %d\n", class_index);
  fwrite(output, strlen(output), 1, fp_output);
  fwrite("call Memory.alloc 1\n", strlen("call Memory.alloc 1\n"), 1, fp_output);
  fwrite("pop pointer 0\n", strlen("pop pointer 0\n"), 1, fp_output);
}

void write_pop_name(char *name) {
  char index[BUFFER];
  Kind kind = kind_of(name);
  sprintf(index, "%d", index_of(name));
  printf("pop: name = %s, kind = %d, index = %s FIELD = %d\n", name, kind, index, FIELD);
  switch(kind) {
    case VAR:
      write_pop(S_LOCAL, index);
      break;
    case ARG:
      write_pop(S_ARG, index);
      break;
    case STATIC:
      write_pop(S_STATIC, index);
      break;
    case FIELD:
      write_pop(S_THIS, index);
      break;
    default:
      printf("error: name = %s\n", name);
      exit(2);
      break;
  }
}

void write_push_name(char *name) {
  char index[BUFFER];
  Kind kind = kind_of(name);
  sprintf(index, "%d", index_of(name));
  switch(kind) {
    case VAR:
      write_push(S_LOCAL, index);
      break;
    case ARG:
      write_push(S_ARG, index);
      break;
    case FIELD:
      write_push(S_THIS, index);
      break;
    case STATIC:
      write_push(S_STATIC, index);
      break;
    case NONE:
      printf("error: name = %s kind = %d\n", name, kind);
      exit(2);
      break;
  }
}

void write_push(Segment segment, char *index) {
  char output[BUFFER] = { '\0' };
  int int_index;
  switch(segment) {
    case S_CONST:
      int_index = atoi(index);
      int is_negative = 0;
      if(int_index < 0) {
        int_index *= -1;
        is_negative = 1;
      }
      sprintf(output, "push constant %d\n", int_index);
      fwrite(output, strlen(output), 1, fp_output);
      if(is_negative) {
        write_arithmetic("neg");
      }
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
    default:
      exit(1);
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
    default:
      exit(1);
      break;
  }
}

void write_label(char *label) {
  char output[BUFFER];
  sprintf(output, "label %s\n", label);
  fwrite(output, strlen(output), 1, fp_output);
}

void write_goto(char *label) {
  char output[BUFFER];
  sprintf(output, "goto %s\n", label);
  fwrite(output, strlen(output), 1, fp_output);
}

void write_if(char *label) {
  char output[BUFFER];
  sprintf(output, "if-goto %s\n", label);
  fwrite(output, strlen(output), 1, fp_output);
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
  } else if (strstr(op, "-") != NULL) {
    printf("sub \n");
    fwrite("sub\n", strlen("sub\n"), 1, fp_output);
  } else if (strstr(op, "=") != NULL) {
    printf("eq \n");
    fwrite("eq\n", strlen("eq\n"), 1, fp_output);
  } else if (strstr(op, "neg") != NULL) {
    printf("neg \n");
    fwrite("neg\n", strlen("neg\n"), 1, fp_output);
  } else if (strstr(op, "&gt;") != NULL) {
    printf("gt \n");
    fwrite("gt\n", strlen("gt\n"), 1, fp_output);
  } else if (strstr(op, "&lt;") != NULL) {
    printf("lt \n");
    fwrite("lt\n", strlen("lt\n"), 1, fp_output);
  } else if (strstr(op, "&amp;") != NULL) {
    printf("and \n");
    fwrite("and\n", strlen("and\n"), 1, fp_output);
  } else if (strstr(op, "||") != NULL) {
    printf("or \n");
    fwrite("or\n", strlen("or\n"), 1, fp_output);
  } else if (strstr(op, "*") != NULL) {
    printf("mult \n");
    fwrite("call Math.multiply 2\n", strlen("call Math.multiply 2\n"), 1, fp_output);
  } else if (strstr(op, "/") != NULL) {
    printf("divide \n");
    fwrite("call Math.divide 2\n", strlen("call Math.multiply 2\n"), 1, fp_output);
  } else if (strstr(op, "~") != NULL) {
    printf("not \n");
    fwrite("not\n", strlen("not\n"), 1, fp_output);
  } else {
    printf("arithmetic error: op = %s \n", op);
    exit(1);
  }
}

void write_function(char *name, int n_locals) {
  char function_name[BUFFER] = { '\0' };
  sprintf(function_name, "function %s.%s %d\n", class_name, name, n_locals);
  fwrite(function_name, strlen(function_name), 1, fp_output);
}
