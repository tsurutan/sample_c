void SymbolTable__constructor(void);
void startSubroutine(void);
void define(char *name, char *type, char *kind);
typedef enum { STATIC, FIELD, ARG, VAR, NONE } Kind;
Kind kind_of(char *name);
int index_of(char *name);
void type_of(char *name, char *output);
int exist_table(char *name);
int get_class_index();
