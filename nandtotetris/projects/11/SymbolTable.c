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

void SymbolTable__constructor(void) {
}

void startSubroutine(void) {
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
      break;
    case FIELD:
      printf("field name = %s, type = %s kind = %s ckind = %d\n", name, type, kind, convertedKind);
      break;
    case ARG:
      break;
    case VAR:
      break;
    case NONE:
      break;
  }
}
