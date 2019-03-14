#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 512
#define A_COMMAND 0
#define C_COMMAND 1
#define L_COMMAND 2

typedef struct _symbol {
  char key[BUFFER];
  int address;
} Symbol;

void parser(char *line, char *output);
void parse_label(char *line, char *output);
int parse_command_type(char *line);
int parse_a_command(char *line, char *output);
int parse_c_command(char *line, char *output);
int parse_l_command(char *line, char *output);
void parse_bin(int number, char *output);
void parse_dest(char *line, char *output);
void parse_comp(char *line, char *output);
void parse_jump(char *line, char *output);
void remove_space(char *line, char *output);
int check_ignore(char *line);
int parse_symbol_address(char *line);

Symbol symbols[BUFFER];
int line_count = 0;
int symbol_count = 22;
int label_address = 16;

int main(int argc, char **argv) {
  char *filepath = argv[1];
  char buf[BUFFER], output[BUFFER];
  FILE *in_f, *out_f;
  strcpy(symbols[0].key, "SP");
  symbols[0].address = 0;
  strcpy(symbols[1].key, "LCL");
  symbols[1].address = 1;
  strcpy(symbols[2].key, "ARG");
  symbols[2].address = 2;
  strcpy(symbols[3].key, "THIS");
  symbols[3].address = 3;
  strcpy(symbols[4].key, "THAT");
  symbols[4].address = 4;
  strcpy(symbols[5].key, "R0");
  symbols[5].address = 0;
  strcpy(symbols[6].key, "R1");
  symbols[6].address = 1;
  strcpy(symbols[7].key, "R2");
  symbols[7].address = 2;
  strcpy(symbols[8].key, "R3");
  symbols[8].address = 3;
  strcpy(symbols[9].key, "R4");
  symbols[9].address = 4;
  strcpy(symbols[10].key, "R5");
  symbols[10].address = 5;
  strcpy(symbols[11].key, "R6");
  symbols[11].address = 6;
  strcpy(symbols[12].key, "R7");
  symbols[12].address = 7;
  strcpy(symbols[13].key, "R8");
  symbols[13].address = 8;
  strcpy(symbols[14].key, "R9");
  symbols[14].address = 9;
  strcpy(symbols[15].key, "R10");
  symbols[15].address = 10;
  strcpy(symbols[16].key, "R11");
  symbols[16].address = 11;
  strcpy(symbols[17].key, "R12");
  symbols[17].address = 12;
  strcpy(symbols[18].key, "R13");
  symbols[18].address = 13;
  strcpy(symbols[19].key, "R14");
  symbols[19].address = 14;
  strcpy(symbols[20].key, "R15");
  symbols[20].address = 15;
  strcpy(symbols[21].key, "SCREEN");
  symbols[21].address = 16384;
  strcpy(symbols[22].key, "KBD");
  symbols[22].address = 24576;

  if(argc == 1) { return 0; }
  in_f = fopen(filepath, "r");
  out_f = fopen("Prog.hack", "w+");

  while(fgets(buf, BUFFER, in_f) != NULL) {
    parse_label(buf, output);
  }
  fseek(in_f, 0, SEEK_SET);
  while(fgets(buf, BUFFER, in_f) != NULL) {
    parser(buf, output);
    if(output[0] == '\0') continue;
    fwrite(output, strlen(output), 1, out_f);
  }
  fclose(in_f);
  fclose(out_f);
}

void parse_label(char *line, char *output) {
  char tmp[BUFFER];
  remove_space(line, tmp);
  if(check_ignore(tmp)) return;

  printf("1st parse line = %s\n", tmp);
  if(parse_command_type(tmp) == L_COMMAND) {
    char key[BUFFER];
    int tmp_len = strlen(tmp);
    sprintf(key, "%.*s", tmp_len - 2, tmp + 1);
    strcpy(symbols[symbol_count].key, key);
    symbols[symbol_count].address = line_count;
    printf("key = %s address = %d\n", symbols[symbol_count].key, line_count);
    symbol_count++;
  } else {
    line_count++;
  }
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
      output[0] = '\0';
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
  int parsed_number;
  if(*line == '0') {
    parsed_number = 0;
  } else {
    parsed_number = atoi(line);
    if(parsed_number == 0) {
      parsed_number = parse_symbol_address(line);
    }
  }

  parse_bin(parsed_number, output);
}

int parse_symbol_address(char *line) {
  int has_symbol;
  for(int index = 0; index < symbol_count; index++) {
    if(strcmp(line, symbols[index].key) == 0) {
      printf("symbol来たよ = %s address = %d\n", symbols[index].key, symbols[index].address);
      return symbols[index].address;
    }
  }

  strcpy(symbols[symbol_count].key, line);
  symbols[symbol_count].address = label_address;
  symbol_count++;
  label_address++;
  return symbols[symbol_count - 1].address;
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
  printf("parse comp = %s, pos = %d\n", line, comp_pos);
  if(comp_pos == 1) {
    tmp_comp[0] = line[0];
    tmp_comp[1] = '\0';
  } else {
    strncpy(tmp_comp, line, comp_pos);
  }
  printf("parse comp = %s, pos = %d\n", tmp_comp, comp_pos);
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
