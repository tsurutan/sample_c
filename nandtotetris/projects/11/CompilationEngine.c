#include <stdio.h>
#include <string.h>
#include "CompilationEngine.h"
#include "SymbolTable.h"
#include "VMWriter.h"

#define BUFFER 1024
#define OPS_SIZE 9

FILE *fp_input;
FILE *fp_output;

char ops[OPS_SIZE][BUFFER] = { "+", "-", "*", ">/<", "&amp;", "|", "&lt;", "&gt;", "=" };

int if_label_index = 0;
int while_label_index = 0;
char subroutine_type[BUFFER] = { '\0' };
char class_name[BUFFER];
void debug_print();
void write_to(char *output) {
  fwrite(output, strlen(output), 1, fp_output);
}

void back(char *back_text) {
  fseek(fp_input, -1 * strlen(back_text), SEEK_CUR);
}
void read_to(char *output) {
  fgets(output, BUFFER, fp_input);
  if(output[0] == '\0') exit(1);
}

void read_value_to(char *output) {
  char tmp[BUFFER];
  int is_start = 0, output_index = 0;
  read_to(tmp);
  for(int i = 0; i < strlen(tmp); i++) {
    if(tmp[i] == '>') {
      is_start = 1;
    } else if(tmp[i] == '<' && tmp[i+1] == '/') {
      output[output_index] = '\0';
      break;
    } else if(is_start) {
      output[output_index] = tmp[i];
      output_index++;
    }
  }
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
  char type[BUFFER], kind[BUFFER], name[BUFFER];
  write_to("<classVarDec>\n");
  read_value_to(kind); // static or field
  read_value_to(type); // type
  read_value_to(name); // identifier
  define(name, type, kind);
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_to(buf);
    if(strstr(buf, ",") == NULL) {
      write_to(buf);
      break;
    }
    write_to(buf);
    char second_name[BUFFER] = { '\0' };
    read_value_to(second_name); // identifier
    define(second_name, type, kind);
  }
  write_to("</classVarDec>\n");
}

int compile_parameter_list(int is_show) {
  printf("=====compile parameter list=====\n");
  write_to("<parameterList>\n");
  int param_count = 1;
  if(is_show) {
    char type_1[BUFFER] = { '\0' };
    char var_name_1[BUFFER] = { '\0' };
    read_value_to(type_1); // type
    read_value_to(var_name_1); // varName
    define(var_name_1, type_1, "argument");
    param_count += 1;
    while(1) {
      if(next_check(",")) {
        write_self(); // ,
        char type_2[BUFFER] = { '\0' };
        char var_name_2[BUFFER] = { '\0' };
        read_value_to(type_2); // type
        read_value_to(var_name_2); // varName
        define(var_name_2, type_2, "argument");
        param_count += 1;
      } else {
        break;
      }
    }
  }
  write_to("</parameterList>\n");
  return param_count;
}

int compile_var_dec(char *line) {
  int var_dec_count = 0;
  printf("=====compile var dec=====\n");
  char var_type[BUFFER] = { '\0' };
  write_to("<varDec>\n");
  write_to(line);
  read_value_to(var_type);
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_value_to(buf);
    if(strstr(buf, ";")) {
      break;
    }
    if(strstr(buf, ",") == NULL) {
      define(buf, var_type, "var");
      var_dec_count += 1;
      printf("kitayo ==== %s count = %d \n", buf, var_dec_count);
    }
  }
  write_to("</varDec>\n");
  return var_dec_count;
}

void compile_expression(void);

int compile_expression_list(int is_show) {
  int arg_count = 0;
  printf("=====compile expression list=====\n");
  write_to("<expressionList>\n");
  if(is_show) {
    arg_count += 1;
    compile_expression();
    while(1) {
      char buf[BUFFER] = {'\0'};
      read_to(buf);
      if(strstr(buf, ",")) {
        arg_count += 1;
        write_to(buf); // ,
        compile_expression();
      } else {
        back(buf);
        break;
      }
    }
  }
  write_to("</expressionList>\n");
  return arg_count;
}

void compile_subroutine_call() {
  printf("=====compile subroutine call=====\n");
  char function_name[BUFFER] = { '\0' };
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_value_to(buf);
    if(strstr(buf, "(") == NULL) {
      sprintf(function_name, "%s%s", function_name, buf);
    } else {
      break;
    }
  }
  char next[BUFFER] = { '\0' };
  int arg_count;
  read_to(next);
  back(next);
  if(strstr(next, ")")) {
    arg_count = compile_expression_list(0);
  } else {
    arg_count = compile_expression_list(1);
  }
  write_call(function_name, arg_count);
  write_self(); // )
}

void compile_term() {
  printf("=====compile term=====\n");
  write_to("<term>\n");
  char buf[BUFFER] = { '\0' };
  read_to(buf);
  int is_negative = 0;
  int is_not = 0;
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
      char var_name[BUFFER];
      back(buf);
      read_value_to(var_name); // varName
      write_push_name(var_name);
    }
  } else {
    if(strstr(buf, "(")) {
      write_to(buf); // (
      compile_expression();
      write_self(); // )
    } else if(strstr(buf, "-") || strstr(buf, "~")) {
      char op_val[BUFFER] = { '\0' };
      back(buf);
      read_value_to(op_val);
      printf("nega or not\n");
      if(strstr(op_val, "-")) {
        is_negative = 1;
      } else if(strstr(op_val, "~")) {
        is_not = 1;
      }
      write_to(buf); // - or ~
      compile_term();
    } else {
      char constant_val[BUFFER] = { '\0' };
      back(buf);
      read_value_to(constant_val);
      printf("constant = %s\n", constant_val);
      if(strstr(constant_val, "true")) {
        write_push(S_CONST, "-1");
      } else if(strstr(constant_val, "false") || strstr(constant_val, "null")) {
        write_push(S_CONST, "0");
      } else {
        printf("kitayo = %s\n", constant_val);
        write_push(S_CONST, constant_val);
      }
      write_to(buf); // integerConstant or keywordConstant or symbolConstant
    }
  }
  write_to("</term>\n");
  if(is_negative) {
    write_arithmetic("neg");
  } else if(is_not) {
    write_arithmetic("~");
  }
  printf("=====compile term end=====\n");
}

void compile_expression(void) {
  printf("=====compile expression=====\n");
  write_to("<expression>\n");
  compile_term();
  char op[BUFFER] = { '\0' };
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_to(buf);
    back(buf);
    if(is_ops(buf)) {
      read_value_to(op);
      compile_term();
      write_arithmetic(op);
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
  write_pop(S_TEMP, "0"); // 捨てる
  write_to("</doStatement>\n");
}

void compile_return_statement(void) {
  printf("=====return statement=====\n");
  write_to("<returnStatement>\n");
  write_self(); // return
  if(!next_check(";")) {
    compile_expression();
  } else {
    write_push(S_CONST, "0");
  }
  write_self(); // ;
  write_return();
  write_to("</returnStatement>\n");
}

void compile_let_statement(void) {
  printf("=====let statement=====\n");
  write_to("<letStatement>\n");
  char var_name[BUFFER] = { '\0' };
  write_self(); // let
  read_value_to(var_name); // var name
  if(next_check("[")) {
    write_self(); // [
    compile_expression();
    write_self(); // ]
  }
  write_self(); // =
  compile_expression();
  write_self(); // ;
  write_pop_name(var_name);
  write_to("</letStatement>\n");
}

void compile_statements(void);

/**
 * label L1
 *  ~(cond)
 *  if-goto L2
 *  s1
 *  goto L1
 * label L2
 */
void compile_while_statement(void) {
  char label_start_name[BUFFER];
  char label_end_name[BUFFER];
  sprintf(label_start_name, "WIHLE_EXP%d", while_label_index);
  sprintf(label_end_name, "WHILE_END%d", while_label_index);
  while_label_index += 1;
  printf("=====while statement=====\n");
  write_to("<whileStatement>\n");
  write_self(); // while
  write_label(label_start_name); // label L1
  write_self(); // (
  compile_expression(); // cond
  write_self(); // )
  write_arithmetic("~"); // 否定
  write_if(label_end_name); // if-goto L2
  write_self(); // {
  compile_statements(); // s1
  write_self(); // }
  write_goto(label_start_name); // goto L1
  write_label(label_end_name);
  write_to("</whileStatement>\n");
}

/**
 * (cond)
 * if-goto TRUE
 * goto FALSE
 * label TRUE
 * s1
 * goto END
 * label FALSE
 *  s2
 * label END
 * */
void compile_if_statement(void) {
  char label_false_name[BUFFER];
  char label_true_name[BUFFER];
  char label_end_name[BUFFER];
  sprintf(label_false_name, "IF_FALSE%d", if_label_index);
  sprintf(label_true_name, "IF_TRUE%d", if_label_index);
  sprintf(label_end_name, "IF_END%d", if_label_index);
  if_label_index += 1;
  printf("=====if statement=====\n");
  write_to("<ifStatement>\n");
  write_self(); // if
  write_self(); // (
  compile_expression(); // cond
  write_self(); // )
  write_if(label_true_name); // if-goto TRUE
  write_goto(label_false_name); // if-goto FALSE
  write_label(label_true_name); // label true
  write_self(); // {
  compile_statements(); // s1
  write_self(); // }
  write_goto(label_end_name); // goto END
  write_label(label_false_name); // label false
  if(next_check("else")) {
    printf("else comming !!!");
    write_self(); // else
    write_self(); // {
    compile_statements(); // s2
    write_self(); // }
  }
  write_label(label_end_name); // label end
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

void compile_subroutine_body(char *subroutine_name) {
  int var_dec_count = 0;
  printf("=====compile subroutine body=====\n");
  write_to("<subroutineBody>\n");
  write_self(); // {
  while(1) {
    char buf[BUFFER] = { '\0' };
    read_to(buf);
    if(strstr(buf, "var")) {
      var_dec_count += compile_var_dec(buf);
    } else {
      write_function(subroutine_name, var_dec_count);
      back(buf);
      compile_statements();
      break;
    }
  }
  write_self(); // }
  write_to("</subroutineBody>\n");
}

void compile_subroutine_dec(void) {
  char subroutine_name[BUFFER];
  startSubroutine();
  printf("=====subroutine dec=====\n");
  write_to("<subroutineDec>\n");
  write_self(); // constructor or function or method
  read_value_to(subroutine_type); // void or type
  read_value_to(subroutine_name);
  write_self(); // (
  if(next_check(")")) {
    compile_parameter_list(0);
  } else {
    compile_parameter_list(1);
  }
  write_self(); // )
  compile_subroutine_body(subroutine_name);
  write_to("</subroutineDec>\n");
}

void compile_class(void) {
  printf("=====compile class=====\n");
  write_to("<class>\n");
  write_self();
  read_value_to(class_name); // class name
  write_self();
  printf("class name = %s\n", class_name);
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
