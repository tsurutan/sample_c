typedef enum _Segment {
  S_CONST,
  S_ARG,
  S_LOCAL,
  S_STATIC,
  S_THIS,
  S_THAT,
  S_POINTER,
  S_TEMP
} Segment;
void VMWriter__constructor(char *name);
void write_function(char *name, int n_locales);
void VMWriter__close(void);
void write_push(Segment segment, char *index);
void write_pop(Segment segment, char *index);
void write_arithmetic(char *op);
void write_call(char *name, int n_locals);
void write_return();
