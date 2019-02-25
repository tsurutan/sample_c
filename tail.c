#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE
#include <getopt.h>
#include <sys/stat.h>

static void do_tail(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

static struct option longopts[] = {
  { "lines", required_argument, NULL, 'n' },
  { "help", no_argument, NULL, 'h' },
  { 0, 0, 0, 0 }
};

int main(int argc, char *argv[]) {
  int opt;
  long nlines = DEFAULT_N_LINES;
  while((opt = getopt_long(argc, argv, "n:", longopts, NULL)) != -1) {
    switch(opt) {
      case 'n':
        nlines = atol(optarg);
        break;
      case 'h':
        fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
        exit(0);
      case '?':
        fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
        exit(1);
    }
  }
  if (optind == argc) {
    do_tail(stdin, nlines);
  } else {
    int i;
    for(i = optind; i < argc; i++) {
      FILE *f;
      struct stat stat_buf;
      f = fopen(argv[i], "r");
      if(!f) {
        exit(1);
      }
      do_tail(f, nlines);
      fclose(f);
    }
  }
  exit(0);
}

static void do_tail(FILE *f, long nlines) {
  int c;
  char buf[100][1024];
  int line = 0;
  int c_count = 0;
  while((c = getc(f)) != EOF) {
    if (c == '\n') {
      buf[line][c_count] = c;
      line++;
      c_count = 0;
    } else {
      buf[line][c_count] = c;
      c_count++;
    }
  }

  for(int i = (line - nlines); i < line; i++) {
    fputs(buf[i], stdout);
  }
}

/* static void do_tail(FILE *f, long nlines) { */
/*   int c; */
/*   int offset = -1; */
/*   int offset_size = 0; */
/*   char tmp[512] = { '\0' }; */
/*   int c_position = 0; */
/*   if(nlines <= 0) return; */
/*   while((c = fgetc(f)) != EOF) { */
/*     offset_size++; */
/*   } */
/*   fseek(f, offset, SEEK_END); */
/*   while((c = getc(f)) != EOF) { */
/*     if (c == '\n') { */
/*       int tmp_i = 0; */
/*       int tmp_j = 0; */
/*       while (tmp[tmp_j] != '\0') { */
/*         #<{(| putchar(tmp[tmp_j]); |)}># */
/*         tmp_j++; */
/*       } */
/*       for(tmp_i = tmp_j - 1; tmp_i >= 0; tmp_i--) { */
/*         putchar(tmp[tmp_i]); */
/*       } */
/*       putchar('\n'); */
/*       tmp[0] = '\0'; */
/*       c_position = 0; */
/*     } else { */
/*       tmp[c_position] = c; */
/*       c_position++; */
/*     } */
/*     offset--; */
/*     offset_size--; */
/*     if(offset_size < 0) return; */
/*     fseek(f, offset, SEEK_END); */
/*   } */
/* } */
