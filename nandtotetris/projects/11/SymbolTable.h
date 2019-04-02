void SymbolTable__constructor(void);
void startSubroutine(void);
void define(char *name, char *type, char *kind);
typedef enum { STATIC, FIELD, ARG, VAR, NONE } Kind;
