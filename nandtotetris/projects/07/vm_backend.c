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
void parse_arithmetic(char *input, char *output);
void set_sp(int sp_address, char *input, char *output);

int stack_address = 256;

int main(int argc, char **argv) {
  char *filepath = argv[1];
  char buf[BUFFER], output[BUFFER] = {0}, output_file[BUFFER];
  FILE *in_f, *out_f;

  if(argc == 1) { return 0; }
  in_f = fopen(filepath, "r");
  filepath[strlen(filepath) - 3] = '\0';
  sprintf(output_file, "%s.asm", filepath);
  out_f = fopen(output_file, "w+");

  while(fgets(buf, BUFFER, in_f) != NULL) {
    parser(buf, output);
    if(output[0] == '\0') continue;
    fwrite(output, strlen(output), 1, out_f);
  }
  fclose(in_f);
  fclose(out_f);
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
      printf("======C_ARITHMETIC======\n%s\n", output);
      break;
    case C_PUSH:
      parse_push(tmp, output);
      printf("======C_PUSH======\n%s\n", output);
      break;
    case C_POP:
      printf("C_POP = %s\n", tmp);
      break;
    case C_LABEL:
      printf("C_LABEL = %s\n", tmp);
      break;
    case C_GOTO:
      printf("C_GOTO = %s\n", tmp);
      break;
    case C_IF:
      printf("C_IF = %s\n", tmp);
      break;
    case C_FUNCTION:
      printf("C_FUNCTION = %s\n", tmp);
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

void parse_push(char *input, char *output) {
  char arg1[BUFFER] = {'\0'}, arg2[BUFFER] = {'\0'};
  parse_arg1(input, arg1);
  parse_arg2(input, arg2);
  printf("PUSH: input = %s, arg1 = %s, arg2 = %s\n", input, arg1, arg2);
  if(strcmp("constant", arg1) == 0) {
    sprintf(output, "@%s\nD=A\n@%d\nM=D\n", arg2, stack_address);
    stack_address++;
  }
  set_sp(stack_address, output, output);
}

void parse_arithmetic(char *input, char *output) {
  if(strcmp("add", input) == 0) {
    int top_stack = stack_address - 1;
    int prev_stack = stack_address - 2;
    sprintf(output, "@%d\nD=M\n@%d\nM=D+M\n", top_stack, prev_stack);
    stack_address--;
  }
  set_sp(stack_address, output, output);
}

void set_sp(int sp_address, char *input, char *output) {
  sprintf(output, "%s@%d\nD=A\n@SP\nM=D\n", input, sp_address);
}
