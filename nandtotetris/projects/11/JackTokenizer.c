#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "JackTokenizer.h"

#define BUFFER 1024

#define TOKEN_TYPE_KEYWORD 1
#define TOKEN_TYPE_SYMBOL 2
#define TOKEN_TYPE_IDENTIFIER 3
#define TOKEN_TYPE_INT_CONST 4
#define TOKEN_TYPE_STRING_CONST 5
#define TOKEN_TYPE_UNDEFINED 6

#define SIZE_KEYWORD 21
#define SIZE_SYMBOL 19

char keywords[SIZE_KEYWORD][BUFFER] = {
  "class",
  "constructor",
  "function",
  "method",
  "field",
  "static",
  "var",
  "int",
  "char",
  "boolean",
  "void",
  "true",
  "false",
  "null",
  "this",
  "let",
  "do",
  "if",
  "else",
  "while",
  "return"
};

char symbols[SIZE_SYMBOL][BUFFER] = {
  "{",
  "}",
  "(",
  ")",
  "[",
  "]",
  ".",
  ",",
  ";",
  "+",
  "-",
  "*",
  "/",
  "&",
  "|",
  "<",
  ">",
  "=",
  "~"
};

FILE *fp_input, *fp_output;

int token_type(char *token) {
  int is_number_const = 1;
  for(int i = 0; i < SIZE_SYMBOL; i++) {
    if(strcmp(token, symbols[i]) == 0) return TOKEN_TYPE_SYMBOL;
  }
  for(int i = 0; i < SIZE_KEYWORD; i++) {
    if(strcmp(token, keywords[i]) == 0) return TOKEN_TYPE_KEYWORD;
  }
  for(int i = 0; token[i] != '\0'; i++) {
    if(token[i] > '9' || token[i] < '0') {
      is_number_const = 0;
      break;
    }
  }
  if(is_number_const) return TOKEN_TYPE_INT_CONST;
  if(token[0] == '"') return TOKEN_TYPE_STRING_CONST;
  if(token[0] == '/') return TOKEN_TYPE_UNDEFINED;
  return TOKEN_TYPE_IDENTIFIER;
}

int is_symbol(char token) {
  for(int i = 0; i < SIZE_SYMBOL; i++) {
    if(token == symbols[i][0]) return 1;
  }
  return 0;
}

void parse_token(char *token, int token_type, char *output) {
  char string_const[BUFFER] = { '\0' };
  switch(token_type) {
    case TOKEN_TYPE_SYMBOL:
      if(token[0] == '<') {
        sprintf(output, "<symbol>&lt;</symbol>\n");
      } else if(token[0] == '&') {
        sprintf(output, "<symbol>&amp;</symbol>\n");
      } else if(token[0] == '>') {
        sprintf(output, "<symbol>&gt;</symbol>\n");
      } else {
        sprintf(output, "<symbol>%s</symbol>\n", token);
      }
      break;
    case TOKEN_TYPE_KEYWORD:
      sprintf(output, "<keyword>%s</keyword>\n", token);
      break;
    case TOKEN_TYPE_INT_CONST:
      sprintf(output, "<integerConstant>%s</integerConstant>\n", token);
      break;
    case TOKEN_TYPE_STRING_CONST:
      for(int i = 1, j = 0; i < strlen(token) - 1; i++, j++) {
        string_const[j] = token[i];
      }
      sprintf(output, "<stringConstant>%s</stringConstant>\n", string_const);
      break;
    case TOKEN_TYPE_IDENTIFIER:
      sprintf(output, "<identifier>%s</identifier>\n", token);
      break;
    case TOKEN_TYPE_UNDEFINED:
      return;
    default:
      printf("token = %s", token);
      perror("error occurred");
      exit(2);
      break;
  }
  printf("output = %s\n", output);
  fwrite(output, strlen(output), 1, fp_output);
}

void JackTokenizer_constructor(FILE *input, FILE *output) {
  char buf[BUFFER] = {0}, token[BUFFER] = {0};
  int token_position = 0;
  int is_skip = 0;
  int is_string = 0;
  fp_input = input;
  fp_output = output;
  fwrite("<tokens>\n", strlen("<tokens>\n"), 1, fp_output);
  while(fgets(buf, BUFFER, fp_input) != NULL) {
    char output[BUFFER] = {0};
    for(int i = 0; i < BUFFER && buf[i] != '\0'; i++) {
      if(buf[i] == '/' && buf[i + 1] == '/') break;
      if(buf[i] == '/' && buf[i + 1] == '*' && buf[i + 2] == '*') {
        is_skip = 1;
      }
      if(is_skip) {
        if(buf[i] == '*' && buf[i + 1] == '/') {
          is_skip = 0;
          break;
        }
        continue;
      }
      if(!is_string && buf[i] == '"') {
        is_string = 1;
        token[token_position] = buf[i];
        token_position++;
        continue;
      } else if(is_string) {
        if(buf[i] == '"') {
          is_string = 0;
        } else {
          token[token_position] = buf[i];
          token_position++;
          continue;
        }
      }
      token[token_position] = '\0';
      if(is_symbol(buf[i])) {
        char token2[2] = {buf[i], '\0'};
        if(token[0] != '\0') {
          parse_token(token, token_type(token), output);
        }
        parse_token(token2, token_type(token2), output);
        token_position = 0;
        token[0] = '\0';
      } else if(isspace(buf[i])) {
        if(token[0] == '\0') continue;
        token_position = 0;
        parse_token(token, token_type(token), output);
        token[0] = '\0';
      } else {
        token[token_position] = buf[i];
        token_position++;
      }
    }
  }
  fwrite("</tokens>\n", strlen("</tokens>\n"), 1, fp_output);
}
