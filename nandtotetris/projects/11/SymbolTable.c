#include <string.h>
#include "SymbolTable.h"
#define BUFFER 512

typedef struct {
  char name[BUFFER];
  char type[BUFFER];
  Kind kind;
  int index;
} Table;

typedef struct {
  Table tables[BUFFER];
} ClassHashTable;

typedef struct {
  Table tables[BUFFER];
} SubroutineHashTable;

ClassHashTable classHashTable;
SubroutineHashTable subroutineHashTable;

void SymbolTable__constructor(void);
void startSubroutine(void);
void define(char *name, char *type, char *kind);
void varCount(Kind kind);
Kind kindOf(char *name);
void typeOf(char *name);
int indexOf(char *name);
Kind convertKind(char *kind);

int static_index = 0;
int field_index = 0;
int var_index = 0;
int argument_index = 0;
int subroutine_index = 0;
int class_index = 0;

void SymbolTable__constructor(void) {
}

void startSubroutine(void) {
  var_index = 0;
  argument_index = 0;
  for(int i = 0; i < subroutine_index + 1; i++) {
    Table table;
    subroutineHashTable.tables[i] = table;
  }
  subroutine_index = 0;
}

Kind kind_of(char *name) {
  for(int i = 0; i < subroutine_index; i++) {
    Table table = subroutineHashTable.tables[i];
    if(strstr(table.name, name)) {
      return table.kind;
    }
  }
  for(int i = 0; i < class_index; i++) {
    Table table = classHashTable.tables[i];
    if(strstr(table.name, name)) {
      return table.kind;
    }
  }
  return NONE;
}

int index_of(char *name) {
  for(int i = 0; i < subroutine_index; i++) {
    Table table = subroutineHashTable.tables[i];
    if(strstr(table.name, name)) {
      return table.index;
    }
  }
  for(int i = 0; i < class_index; i++) {
    Table table = classHashTable.tables[i];
    if(strstr(table.name, name)) {
      return table.index;
    }
  }
  return -1;
}

void type_of(char *name, char *output) {
  for(int i = 0; i < subroutine_index; i++) {
    Table table = subroutineHashTable.tables[i];
    printf("table name = %s\n", subroutineHashTable.tables[i].name);
    if(strstr(table.name, name)) {
      strcpy(output, table.type);
      return;
    }
  }
  for(int i = 0; i < class_index; i++) {
    Table table = classHashTable.tables[i];
    if(strstr(table.name, name)) {
      strcpy(output, table.type);
      return;
    }
  }
}

Kind convertKind(char *kind) {
  if(strstr(kind, "static")) {
    return STATIC;
  } else if(strstr(kind, "field")) {
    return FIELD;
  } else if(strstr(kind, "var")) {
    return VAR;
  } else if(strstr(kind, "argument")) {
    return ARG;
  }
  return NONE;
}

void define(char *name, char *type, char *kind) {
  Kind convertedKind = convertKind(kind);

  switch(convertedKind) {
    case STATIC:
      printf("static name = %s, type = %s kind = %s\n", name, type, kind);
      Table static_table;
      strcpy(static_table.name, name);
      strcpy(static_table.type, type);
      static_table.kind = convertedKind;
      static_table.index = static_index;
      subroutineHashTable.tables[class_index] = static_table;
      static_index++;
      class_index++;
      break;
    case FIELD:
      printf("field name = %s, type = %s kind = %s\n", name, type, kind);
      Table field_table;
      strcpy(field_table.name, name);
      strcpy(field_table.type, type);
      field_table.kind = convertedKind;
      field_table.index = field_index;
      subroutineHashTable.tables[class_index] = field_table;
      field_index++;
      class_index++;
      break;
    case ARG:
      printf("argument name = %s, type = %s kind = %s\n", name, type, kind);
      Table argument_table;
      strcpy(argument_table.name, name);
      strcpy(argument_table.type, type);
      argument_table.kind = convertedKind;
      argument_table.index = argument_index;
      subroutineHashTable.tables[subroutine_index] = argument_table;
      argument_index++;
      subroutine_index++;
      break;
    case VAR:
      printf("var name = %s, type = %s kind = %s\n", name, type, kind);
      Table var_table;
      strcpy(var_table.name, name);
      strcpy(var_table.type, type);
      var_table.kind = convertedKind;
      var_table.index = var_index;
      subroutineHashTable.tables[subroutine_index] = var_table;
      var_index++;
      subroutine_index++;
      break;
    case NONE:
      break;
  }
}
