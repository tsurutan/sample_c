#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <getopt.h>

static void do_head(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

int is_tab = 0;
int is_nline = 0;
static struct option longopts[] = {
  { "tab", required_argument, &is_tab, 1 },
  { "nline", no_argument, &is_nline, 1 },
  { 0, 0, 0, 0 }
};

int main(int argc, char *argv[]) {
  int opt;
  long nlines = DEFAULT_N_LINES;
  while((opt = getopt_long(argc, argv, "nt", longopts, NULL)) != -1) {
    switch(opt) {
      case 'n':
        is_nline = 1;
        break;
      case 't':
        is_tab = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
        exit(1);
    }
  }
  if (optind == argc) {
    do_head(stdin, nlines);
  } else {
    int i;
    for(i = optind; i < argc; i++) {
      FILE *f;
      f = fopen(argv[i], "r");
      if(!f) {
        exit(1);
      }
      do_head(f, nlines);
      fclose(f);
    }
  }
  exit(0);
}

static void do_head(FILE *f, long nlines) {
  int c;
  int tab_count = 0;
  int nline_count = 0;
  if(nlines <= 0) return;
  while((c = getc(f)) != EOF) {
    if(c == '\t') tab_count++;
    if(c == '\n') nline_count++;
    if(putchar(c) < 0) exit(1);
  }
  if(is_tab) printf("tab count = %d\n", tab_count);
  if(is_nline) printf("nline count = %d\n", nline_count);
}
