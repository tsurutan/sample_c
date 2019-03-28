#include <stdio.h>
#include <string.h>
#include "CompilationEngine.h"

#define BUFFER 1024
#define OPS_SIZE 9

FILE *fp_input;
FILE *fp_output;

char ops[OPS_SIZE][BUFFER] = { "+", "-", "*", ">/<", "&amp;", "|", "&lt;", "&gt;", "=" };

void debug_print();
void write_to(char *output) {
  fwrite(output, strlen(output), 1, fp_output);
  printf("write = %s\n", output);
}

void back(char *back_text) {
  fseek(fp_input, -1 * strlen(back_text), SEEK_CUR);
}

void read_to(char *output) {
  fgets(output, BUFFER, fp_input);
  if(output[0] == '\0') exit(1);
}

void write_self() {
  char buf[BUFFER] = { '\0' };
  read_to(buf);
  write_to(buf);
}

int next_check(char *line) {
  char buf[BUFFER] = { '\0' };
  read_to(buf);
  back(buf);
  return strstr(buf, line) == NULL ? 0 : 1;
}

void debug_print() {
  char buf[BUFFER] = {'\0'};
  read_to(buf);
  printf("kitayo = %s\n", buf);
  back(buf);
}

int is_ops(char *line) {
  for(int i = 0; i < OPS_SIZE; i++) {
    if(strstr(line, ops[i]) != NULL) return 1;
  }
  return 0;
}
void compile_class_var_dec(void) {
  printf("=====compile class var dec=====\n");
  write_to("<classVarDec>\n");
  write_self(); // var
  write_self();
  write_self();
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_to(buf);
    if(strstr(buf, ",") == NULL) {
      write_to(buf);
      break;
    }
    write_to(buf);
    write_self();
  }
  write_to("</classVarDec>\n");
}

void compile_parameter_list(int is_show) {
  printf("=====compile parameter list=====\n");
  write_to("<parameterList>\n");
  if(is_show) {
    write_self(); // type
    write_self(); // varName
    while(1) {
      if(next_check(",")) {
        write_self(); // ,
        write_self(); // type
        write_self(); // varName
      } else {
        break;
      }
    }
  }
  write_to("</parameterList>\n");
}

void compile_var_dec(char *line) {
  printf("=====compile var dec=====\n");
  write_to("<varDec>\n");
  write_to(line);
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_to(buf);
    write_to(buf);
    if(strstr(buf, ";")) {
      break;
    }
  }
  write_to("</varDec>\n");
}

void compile_expression(void);

void compile_expression_list(int is_show) {
  printf("=====compile expression list=====\n");
  write_to("<expressionList>\n");
  if(is_show) {
    compile_expression();
    while(1) {
      char buf[BUFFER] = {'\0'};
      read_to(buf);
      if(strstr(buf, ",")) {
        write_to(buf); // ,
        compile_expression();
      } else {
        back(buf);
        break;
      }
    }
  }
  write_to("</expressionList>\n");
}

void compile_subroutine_call() {
  printf("=====compile subroutine call=====\n");
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_to(buf);
    write_to(buf);
    if(strstr(buf, "(")) break;
  }
  char next[BUFFER] = { '\0' };
  read_to(next);
  back(next);
  if(strstr(next, ")")) {
    compile_expression_list(0);
  } else {
    compile_expression_list(1);
  }
  write_self(); // )
}

void compile_term() {
  printf("=====compile term=====\n");
  write_to("<term>\n");
  char buf[BUFFER] = { '\0' };
  read_to(buf);
  if(strstr(buf, "identifier")) {
    char next[BUFFER] = { '\0' };
    read_to(next);
    back(next);
    if(strstr(next, "[")) {
      write_to(buf); // varName
      write_self(); // [
      compile_expression();
      write_self(); // ]
    } else if (strstr(next, "(") || strstr(next, ".")) {
      back(buf);
      compile_subroutine_call();
    } else {
      write_to(buf); // varName
    }
  } else {
    if(strstr(buf, "(")) {
      write_to(buf); // (
      compile_expression();
      write_self(); // )
    } else if(strstr(buf, "-") || strstr(buf, "~")) {
      write_to(buf); // - or ~
      compile_term();
    } else {
      write_to(buf); // integerConstant or keywordConstant or symbolConstant
    }
  }
  write_to("</term>\n");
  printf("=====compile term end=====\n");
}

void compile_expression(void) {
  printf("=====compile expression=====\n");
  write_to("<expression>\n");
  compile_term();
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_to(buf);
    back(buf);
    if(is_ops(buf)) {
      write_self(); // op
      compile_term();
    } else {
      break;
    }
  }
  write_to("</expression>\n");
}

void compile_do_statement(void) {
  printf("=====do statement=====\n");
  write_to("<doStatement>\n");
  write_self(); // do
  compile_subroutine_call();
  write_self(); // ;
  write_to("</doStatement>\n");
}

void compile_return_statement(void) {
  printf("=====return statement=====\n");
  write_to("<returnStatement>\n");
  write_self(); // return
  if(!next_check(";")) {
    compile_expression();
  }
  write_self(); // ;
  write_to("</returnStatement>\n");
}

void compile_let_statement(void) {
  printf("=====let statement=====\n");
  write_to("<letStatement>\n");
  write_self(); // let
  write_self(); // varName
  if(next_check("[")) {
    write_self(); // [
    compile_expression();
    write_self(); // ]
  }
  write_self(); // =
  compile_expression();
  write_self(); // ;
  write_to("</letStatement>\n");
}

void compile_statements(void);

void compile_while_statement(void) {
  printf("=====while statement=====\n");
  write_to("<whileStatement>\n");
  write_self(); // while
  write_self(); // (
  compile_expression();
  write_self(); // )
  write_self(); // {
  compile_statements();
  write_self(); // }
  write_to("</whileStatement>\n");
}

void compile_if_statement(void) {
  printf("=====if statement=====\n");
  write_to("<ifStatement>\n");
  write_self(); // if
  write_self(); // (
  compile_expression();
  write_self(); // )
  write_self(); // {
  compile_statements();
  write_self(); // }
if(next_check("else")) {
  printf("else comming !!!");
  write_self(); // else
  write_self(); // {
  compile_statements();
  write_self(); // }
}
write_to("</ifStatement>\n");
}

void compile_statements(void) {
  printf("=====statements=====\n");
  write_to("<statements>\n");
  while(1) {
    if(next_check(">let</")) {
      compile_let_statement();
    } else if(next_check(">return</")) {
      compile_return_statement();
    } else if(next_check(">do</")) {
      compile_do_statement();
    } else if(next_check(">if</")) {
      compile_if_statement();
    } else if(next_check(">while</")) {
      compile_while_statement();
    } else {
      break;
    }
  }
  write_to("</statements>\n");
}

void compile_subroutine_body(void) {
  printf("=====compile subroutine body=====\n");
  write_to("<subroutineBody>\n");
  write_self(); // {
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_to(buf);
    if(strstr(buf, "var")) {
      compile_var_dec(buf);
    } else {
      back(buf);
      compile_statements();
      break;
    }
  }
  write_self(); // }
  write_to("</subroutineBody>\n");
}

void compile_subroutine_dec(void) {
  printf("=====subroutine dec=====\n");
  write_to("<subroutineDec>\n");
  write_self(); // constructor or function or method
  write_self(); // void or type
  write_self(); // subroutineName
  write_self(); // (
  if(next_check(")")) {
    compile_parameter_list(0);
  } else {
    compile_parameter_list(1);
  }
  write_self(); // )
  compile_subroutine_body();
  write_to("</subroutineDec>\n");
}

void compile_class(void) {
  printf("=====compile class=====\n");
  write_to("<class>\n");
  write_self();
  write_self();
  write_self();
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_to(buf);
    back(buf);
    if(strstr(buf, "static") || strstr(buf, "field")) {
      compile_class_var_dec();
    } else if(strstr(buf, "constructor") || strstr(buf, "function") || strstr(buf, "method")) {
      compile_subroutine_dec();
    } else if(strstr(buf, "}")) {
      write_self(); // }
      break;
    }
  }
  write_to("</class>\n");
}

void CompilationEngine_constructor(FILE *input, FILE *output) {
  fp_input = input;
  fp_output = output;
  char tmp[BUFFER];
  read_to(tmp);
  compile_class();
}
