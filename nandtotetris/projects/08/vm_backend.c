#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 1024
#define C_ARITHMETIC 0
#define C_PUSH 1
#define C_POP 2
#define C_LABEL 3
#define C_GOTO 4
#define C_IF 5
#define C_FUNCTION 6
#define C_RETURN 7
#define C_CALL 8

typedef struct _symbol {
  char key[BUFFER];
  int address;
} Symbol;

void parser(char *line, char *output);
int parse_command_type(char *line);
void remove_space(char *line, char *output);
int check_ignore(char *line);
int parse_symbol_address(char *line);
int starts_with(char *pre, char *target);
void parse_arg1(char *line, char *output);
void parse_arg2(char *line, char *output);

void parse_push(char *input, char *output);
void parse_pop(char *input, char *output);
void parse_label(char *input, char *output);
void parse_if(char *input, char *output);
void parse_goto(char *input, char *output);
void parse_arithmetic(char *input, char *output);
void parse_compare(char *compare, char *output);
void parse_function(char *input, char *output);
void parse_return(char *input, char *output);
void set_args(int arg1_index, int arg2_index, char *output);
void set_sp(int sp_address, char *input, char *output);
void set_segment(char *arg1, int position, char *output);
void substitution(char *label, int value, char *output);
void pop(char *output);
void pop_with_m(char *output);
void push(char *output);

int label_index = 0;

int base_pointer = 3;
int base_temp = 5;
int base_static = 16;
int base_lcl = 300;
int base_arg = 400;
int base_this = 3000;
int base_that = 3010;

int tmp_register = 13;

int main(int argc, char **argv) {
  char *filepath = argv[1];
  char buf[BUFFER], output[BUFFER] = {0}, output_file[BUFFER];
  FILE *in_f, *out_f;

  if(argc == 1) { return 0; }
  in_f = fopen(filepath, "r");
  filepath[strlen(filepath) - 3] = '\0';
  sprintf(output_file, "%s.asm", filepath);
  out_f = fopen(output_file, "w+");

  /* substitution("LCL", base_lcl, output); */
  /* substitution("ARG", base_arg, output); */
  /* substitution("THIS", base_this, output); */
  /* substitution("THAT", base_that, output); */
  /* fwrite(output, strlen(output), 1, out_f); */
  while(fgets(buf, BUFFER, in_f) != NULL) {
    char line_output[BUFFER] = {0};
    parser(buf, line_output);
    if(line_output[0] == '\0') continue;
    fwrite(line_output, strlen(line_output), 1, out_f);
  }
  sprintf(output, "(END)\n@END\n0;JMP\n");
  fwrite(output, strlen(output), 1, out_f);
  fclose(in_f);
  fclose(out_f);
}

// Label = value;
void substitution(char *label, int value, char *output) {
  sprintf(output, "%s@%d\nD=A\n@%s\nM=D\n", output, value, label);
}

void parser(char *line, char *output) {
  char tmp[BUFFER] = {0};
  remove_space(line, tmp);
  if(check_ignore(tmp)) return;

  int command_type = parse_command_type(tmp);
  printf("======command: %s ======\n", tmp);
  switch(command_type) {
    case C_ARITHMETIC:
      parse_arithmetic(tmp, output);
      printf("======C_ARITHMETIC======\n%s", output);
      break;
    case C_PUSH:
      parse_push(tmp, output);
      printf("======C_PUSH======\n%s", output);
      break;
    case C_POP:
      parse_pop(tmp, output);
      printf("======C_POP======\n%s", output);
      break;
    case C_LABEL:
      parse_label(tmp, output);
      printf("======C_LABEL====\n%s", output);
      break;
    case C_GOTO:
      parse_goto(tmp, output);
      printf("======C_GOTO====\n%s", output);
      break;
    case C_IF:
      parse_if(tmp, output);
      printf("======C_IF====\n%s", output);
      break;
    case C_FUNCTION:
      parse_function(tmp, output);
      printf("======C_FUNCTION====\n%s", output);
      break;
    case C_RETURN:
      printf("C_RETURN = %s\n", tmp);
      break;
    case C_CALL:
      printf("C_CALL = %s\n", tmp);
      break;
    deafult:
      break;
  }
}

void remove_space(char *line, char *output) {
  int index = 0, slush = 0, i, j;
  for( i = strlen(line)-1; i >= 0 && isspace( line[i] ); i-- ) ;
  line[i+1] = '\0';
  for( i = 0; isspace(line[i]); i++ ) ;
  j = 0;
  while(line[i]) output[j++] = line[i++];
  output[j] = '\0';
  while(output[index] != '\0') {
    index++;
    if(output[index] == '/') {
      slush++;
      if (slush == 2) break;
    } else {
      slush = 0;
    }
  }
  if(slush == 2) output[index - 2] = '\0';
}

int check_ignore(char *line) {
  if(line[0] == '\0') return 1;
  if(line[0] == '/' && line[1] == '/') return 1;

  return 0;
}

int parse_command_type(char *line) {
  if(starts_with("push", line)) {
    return C_PUSH;
  } else if(starts_with("pop", line)) {
    return C_POP;
  } else if(starts_with("label", line)) {
    return C_LABEL;
  } else if(starts_with("if-goto", line)) {
    return C_IF;
  } else if(starts_with("goto", line)) {
    return C_GOTO;
  } else if(starts_with("function", line)) {
    return C_FUNCTION;
  } else {
    return C_ARITHMETIC;
  }
}

int starts_with(char *pre, char *target) {
  size_t pre_size = strlen(pre), target_size = strlen(target);
  if(pre_size > target_size) return 0;
  return strncmp(pre, target, pre_size) == 0 ? 1 : 0;
}

void parse_arg1(char *line, char *output) {
  int index = 0, output_index = 0;
  while(!isspace(line[index])) { index++; };
  index++;
  while(!isspace(line[index]) && line[index] != '\0') {
    output[output_index] = line[index];
    index++;
    output_index++;
  }
  output[output_index] = '\0';
}

void parse_arg2(char *line, char *output) {
  int index = 0, output_index = 0;
  while(!isspace(line[index])) { index++; };
  index++;
  while(!isspace(line[index])) { index++; };
  index++;
  while(!isspace(line[index]) && line[index] != '\0') {
    output[output_index] = line[index];
    index++;
    output_index++;
  }
  output[output_index] = '\0';
}

void parse_function(char *input, char *output) {
  char arg1[BUFFER] = {'\0'}, arg2[BUFFER] = {'\0'};
  parse_arg1(input, arg1);
  parse_arg2(input, arg2);
  sprintf(output, "(%s)\n", arg1);
  for(int i = 0; i < atoi(arg2); i++) {
    sprintf(output, "%s@LCL\nD=M\n@%d\nA=D+A\nM=0\n", output, i);
  }
}

void parse_return(char *output) {
  sprintf(output, "%s@LCL\nD=M\n@%d\nM=D\nM=0\n", tmp_register);
}

void parse_label(char *input, char *output) {
  char arg1[BUFFER] = {'\0'};
  parse_arg1(input, arg1);
  sprintf(output, "(%s)\n", arg1);
}

void parse_goto(char *input, char *output) {
  char arg1[BUFFER] = {'\0'};
  parse_arg1(input, arg1);
  sprintf(output, "@%s\n0;JMP\n", arg1);
}

void parse_if(char *input, char *output) {
  char arg1[BUFFER] = {'\0'};
  parse_arg1(input, arg1);
  pop(output);
  sprintf(output, "%s@%s\nD;JNE\n", output, arg1);
}

void pop(char *output) {
  sprintf(output, "%s@SP\nM=M-1\nA=M\nD=M\n", output);
}

void pop_with_m(char *output) {
  sprintf(output, "%s@SP\nM=M-1\nA=M\n", output);
}

// すでにDに値が入っているとする
void push(char *output) {
  sprintf(output, "%s@SP\nA=M\nM=D\n@SP\nM=M+1\n", output);
}

void parse_push(char *input, char *output) {
  char arg1[BUFFER] = {'\0'}, arg2[BUFFER] = {'\0'};
  parse_arg1(input, arg1);
  parse_arg2(input, arg2);
  printf("PUSH: input = %s, arg1 = %s, arg2 = %s\n", input, arg1, arg2);
  if(strcmp("constant", arg1) == 0) {
    sprintf(output, "@%s\nD=A\n", arg2);
  } else {
    int position = atoi(arg2);
    set_segment(arg1, position, output);
    sprintf(output, "%s@%d\nA=M\nD=M\n", output, tmp_register);
  }
  push(output);
}

void parse_pop(char *input, char *output) {
  char arg1[BUFFER] = {'\0'}, arg2[BUFFER] = {'\0'};
  parse_arg1(input, arg1);
  parse_arg2(input, arg2);
  int position = atoi(arg2);
  set_segment(arg1, position, output);
  pop(output);
  sprintf(output, "%s@%d\nA=M\nM=D\n", output, tmp_register);
}

// memory[13]にセグメントアドレスを代入する
void set_segment(char *arg1, int position, char *output) {
  if(strcmp("local", arg1) == 0) {
    sprintf(output, "%s@%d\nD=A\n@%s\nD=D+M\n@%d\nM=D\n", output, position, "LCL", tmp_register);
  } else if (strcmp("argument", arg1) == 0) {
    sprintf(output, "%s@%d\nD=A\n@%s\nD=D+M\n@%d\nM=D\n", output, position, "ARG", tmp_register);
  } else if (strcmp("this", arg1) == 0) {
    sprintf(output, "%s@%d\nD=A\n@%s\nD=D+M\n@%d\nM=D\n", output, position, "THIS", tmp_register);
  } else if (strcmp("that", arg1) == 0) {
    sprintf(output, "%s@%d\nD=A\n@%s\nD=D+M\n@%d\nM=D\n", output, position, "THAT", tmp_register);
  } else if (strcmp("temp", arg1) == 0) {
    sprintf(output, "%s@%d\nD=A\n@%d\nM=D\n", output, (base_temp + position), tmp_register);
  } else if (strcmp("pointer", arg1) == 0) {
    sprintf(output, "%s@%d\nD=A\n@%d\nM=D\n", output, (base_pointer + position), tmp_register);
  } else if (strcmp("static", arg1) == 0) {
    sprintf(output, "%s@%d\nD=A\n@%d\nM=D\n", output, (base_static + position), tmp_register);
  } else {
    perror("PARSE POP ERROR");
    exit(2);
  }
}

void parse_arithmetic(char *input, char *output) {
  if(strcmp("neg", input) == 0) {
    pop_with_m(output);
    sprintf(output, "%sD=-M\n", output);
    push(output);
  } else if(strcmp("not", input) == 0) {
    pop_with_m(output);
    sprintf(output, "%sD=!M\n", output);
    push(output);
  } else {
    pop(output);
    pop_with_m(output);
    if(strcmp("add", input) == 0) {
      sprintf(output, "%sD=D+M\n", output);
      push(output);
    } else if(strcmp("sub", input) == 0) {
      sprintf(output, "%sD=D-M\nD=-D\n", output);
      push(output);
    } else if(strcmp("and", input) == 0) {
      sprintf(output, "%sD=D&M\n", output);
      push(output);
    } else if(strcmp("or", input) == 0) {
      sprintf(output, "%sD=D|M\n", output);
      push(output);
    } else if(strcmp("eq", input) == 0) {
      parse_compare("JEQ", output);
    } else if(strcmp("lt", input) == 0) {
      parse_compare("JGT", output);
    } else if(strcmp("gt", input) == 0) {
      parse_compare("JLT", output);
    }
  }
}

void set_args(int arg1_index, int arg2_index, char *output) {
  sprintf(output, "@%d\nD=M\n@%d\n", arg2_index, arg1_index);
}

void set_sp(int sp_address, char *input, char *output) {
  sprintf(output, "%s@%d\nD=A\n@SP\nM=D\n", input, sp_address);
}

// LT GTなどの比較
void parse_compare(char *compare, char *output) {
  sprintf(output, "%sD=D-M\n@TRUE%d\nD;%s\n@SP\nA=M\nM=0\n@END%d\n0;JMP\n(TRUE%d)\n@SP\nA=M\nM=-1\n(END%d)\n@SP\nM=M+1\n",
      output,
      label_index,
      compare,
      label_index,
      label_index,
      label_index);
  label_index++;
}
