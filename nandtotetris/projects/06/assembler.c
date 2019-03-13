#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 512
#define A_COMMAND 0
#define C_COMMAND 1
#define L_COMMAND 2

typedef struct _symbol {
  char *key;
  int address;
} Symbol;

void parser(char *line, char *output);
void parse_label(char *line, char *output);
int parse_command_type(char *line);
int parse_a_command(char *line, char *output);
int parse_c_command(char *line, char *output);
void parse_bin(int number, char *output);
void parse_dest(char *line, char *output);
void parse_comp(char *line, char *output);
void parse_jump(char *line, char *output);
void remove_space(char *line, char *output);
int check_ignore(char *line);

Symbol *symbols[BUFFER];
int line_count = 0;

int main(int argc, char **argv) {
  char *filepath = argv[1];
  char buf[BUFFER], output[BUFFER];
  FILE *in_f, *out_f;

  if(argc == 1) { return 0; }
  in_f = fopen(filepath, "r");
  out_f = fopen("Prog.hack", "w+");

  while(fgets(buf, BUFFER, in_f) != NULL) {
    parse_label(buf, output);
  }
  fseek(in_f, 0, SEEK_SET);
  while(fgets(buf, BUFFER, in_f) != NULL) {
    parser(buf, output);
    fwrite(output, strlen(output), 1, out_f);
  }
  fclose(in_f);
  fclose(out_f);
}

void parse_label(char *line, char *output) {
  char tmp[BUFFER];
  remove_space(line, tmp);
  printf("comment removed = %s\n", tmp);
  if(check_ignore(tmp)) return;
  if(*tmp == '(') {
    char key[BUFFER];
    int tmp_len = strlen(tmp);
    sprintf(key, "%.*s", tmp_len - 2, tmp + 1);
  }
  line_count++;
}

void parser(char *line, char *output) {
  char tmp[BUFFER];
  remove_space(line, tmp);
  if(check_ignore(tmp)) return;

  int command_type = parse_command_type(tmp);
  printf("======command: %s ======\n", tmp);
  switch(command_type) {
    case A_COMMAND:
      parse_a_command(tmp, output);
      printf("accomand %s\n", output);
      break;
    case C_COMMAND:
      parse_c_command(tmp, output);
      printf("ccommand %s\n", output);
      break;
    case L_COMMAND:
      printf("lcommand %s\n", tmp);
      break;
    deafult:
      break;
  }
}

void remove_space(char *line, char *output) {
  int index = 0, slush = 0;
  while(*line != '\0') {
    if(!isspace(*line)) {
      output[index] = *line;
      index++;
    }
    if(*line == '/') {
      slush++;
      if (slush == 2) break;
    } else {
      slush = 0;
    }
    line++;
  }
  output[index] = '\0';
  if(slush == 2) output[index - 2] = '\0';
}

int check_ignore(char *line) {
  if(line[0] == '\0') return 1;
  if(line[0] == '/' && line[1] == '/') return 1;

  return 0;
}

int parse_command_type(char *line) {
  switch(line[0]) {
    case '@':
      return A_COMMAND;
    case '(':
      return L_COMMAND;
    default:
      return C_COMMAND;
  }
}

int parse_a_command(char *line, char *output) {
  line++;

  parse_bin(atoi(line), output);
}

int parse_c_command(char *line, char *output) {
  int has_dest = 0, has_jump = 0, dest_pos = 0, jump_pos = 0, comp_pos = 0;
  char o_dest[4] = "000", o_comp[8] = "0000000", o_jump[4] = "000";
  while(*line != '\0') {
    if(*line == '=') {
      has_dest = 1;
      break;
    }
    line++;
    dest_pos++;
  }
  line-=dest_pos;
  if(has_dest) {
    char tmp_dest[4];
    strncpy(tmp_dest, line, dest_pos);
    parse_dest(tmp_dest, o_dest);
    line+=dest_pos + 1;
  }
  while(*line != '\0' && *line != ';') {
    line++;
    comp_pos++;
  }
  line-=comp_pos;
  char tmp_comp[4];
  strncpy(tmp_comp, line, comp_pos);
  parse_comp(tmp_comp, o_comp);
  line+=comp_pos;
  while(*line != '\0') {
    if(*line == ';') {
      has_jump = 1;
    }
    line++;
    jump_pos++;
  }
  if(has_jump) {
    char tmp_jump[4];
    line-=jump_pos-1;
    strncpy(tmp_jump, line, jump_pos);
    parse_jump(tmp_jump, o_jump);
  }
  sprintf(output, "111%s%s%s\n", o_comp, o_dest, o_jump);
}

void parse_jump(char *line, char *output) {
  if(strcmp(line, "JGT") == 0) {
    strcpy(output, "001");
  } else if (strcmp(line, "JEQ") == 0) {
    strcpy(output, "010");
  } else if (strcmp(line, "JGE") == 0) {
    strcpy(output, "011");
  } else if (strcmp(line, "JLT") == 0) {
    strcpy(output, "100");
  } else if (strcmp(line, "JNE") == 0) {
    strcpy(output, "101");
  } else if (strcmp(line, "JLE") == 0) {
    strcpy(output, "110");
  } else if (strcmp(line, "JMP") == 0) {
    strcpy(output, "111");
  }
}

void parse_comp(char *line, char *output) {
  if(strcmp(line, "0") == 0) {
    strcpy(output, "0101010");
  } else if (strcmp(line, "1") == 0) {
    strcpy(output, "0111111");
  } else if (strcmp(line, "-1") == 0) {
    strcpy(output, "0111010");
  } else if (strcmp(line, "D") == 0) {
    strcpy(output, "0001100");
  } else if (strcmp(line, "A") == 0) {
    strcpy(output, "0110000");
  } else if (strcmp(line, "M") == 0) {
    strcpy(output, "1110000");
  } else if (strcmp(line, "!D") == 0) {
    strcpy(output, "0001101");
  } else if (strcmp(line, "!A") == 0) {
    strcpy(output, "0110001");
  } else if (strcmp(line, "!M") == 0) {
    strcpy(output, "1110001");
  } else if (strcmp(line, "-D") == 0) {
    strcpy(output, "0001111");
  } else if (strcmp(line, "-A") == 0) {
    strcpy(output, "0110011");
  } else if (strcmp(line, "-M") == 0) {
    strcpy(output, "1110011");
  } else if (strcmp(line, "D+1") == 0) {
    strcpy(output, "0011111");
  } else if (strcmp(line, "A+1") == 0) {
    strcpy(output, "0110111");
  } else if (strcmp(line, "M+1") == 0) {
    strcpy(output, "1110111");
  } else if (strcmp(line, "D-1") == 0) {
    strcpy(output, "0001110");
  } else if (strcmp(line, "A-1") == 0) {
    strcpy(output, "0110010");
  } else if (strcmp(line, "M-1") == 0) {
    strcpy(output, "1110010");
  } else if (strcmp(line, "D+A") == 0) {
    strcpy(output, "0000010");
  } else if (strcmp(line, "D+M") == 0) {
    strcpy(output, "1000010");
  } else if (strcmp(line, "D-A") == 0) {
    strcpy(output, "0010011");
  } else if (strcmp(line, "D-M") == 0) {
    strcpy(output, "1010011");
  } else if (strcmp(line, "A-D") == 0) {
    strcpy(output, "0000111");
  } else if (strcmp(line, "M-D") == 0) {
    strcpy(output, "1000111");
  } else if (strcmp(line, "D&A") == 0) {
    strcpy(output, "0000000");
  } else if (strcmp(line, "D&M") == 0) {
    strcpy(output, "1000000");
  } else if (strcmp(line, "D|A") == 0) {
    strcpy(output, "0010101");
  } else if (strcmp(line, "D|M") == 0) {
    strcpy(output, "1010101");
  }
}

void parse_dest(char *line, char *output) {
  int index = 0;
  while(line[index] != '\0') {
    switch(line[index]) {
      case 'A':
        output[0]='1';
        break;
      case 'D':
        output[1]='1';
        break;
      case 'M':
        output[2]='1';
        break;
    }
    index++;
  }
  printf("dest = %s, out = %s\n", line, output);
}

void parse_bin(int number, char *output) {
  int index = 0, mask = 1;
  for(mask=(0x1<<(15)); mask>0; mask>>=1) {
    output[index] = mask&number ? '1' : '0';
    index++;
  }
  output[index] = '\n';
  output[++index] = '\0';
}
