#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#define _GNU_SOURCE
#include <getopt.h>

static void do_grep(regex_t *pat, FILE *f, int reverse);

int main(int argc, char *argv[]) {
  regex_t pat;
  int err;
  int i;
  int ignore = 0;
  int reverse = 0;
  int opt;

  if (argc < 2) {
    fputs("no pattern\n", stderr);
    exit(1);
  }
  while((opt = getopt(argc, argv, "iv")) != -1) {
    switch(opt) {
      case 'i':
        ignore = 1;
        break;
      case 'v':
        reverse = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
        exit(1);
    }
  }
  int flag;
  if(ignore) {
    flag = REG_EXTENDED | REG_NOSUB | REG_NEWLINE | REG_ICASE;
  } else {
    flag = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
  }
  err = regcomp(&pat, argv[optind], flag);
  if(err != 0) {
    char buf[2042];
    regerror(err, &pat, buf, sizeof buf);
    puts(buf);
    exit(1);
  } else {
    for (i = optind + 1; i < argc; i++) {
      FILE *f;
      f = fopen(argv[i], "r");
      if(!f) {
        perror(argv[i]);
        exit(1);
      }
      do_grep(&pat, f, reverse);
      fclose(f);
    }
  }
  regfree(&pat);
  exit(0);
}

static void do_grep(regex_t *pat, FILE *f, int reverse) {
  char buf[4096];
  while(fgets(buf, sizeof buf, f)) {
    if(regexec(pat, buf, 0, NULL, 0) == 0) {
      if(!reverse) fputs(buf, stdout);
    } else {
      if(reverse) fputs(buf, stdout);
    }
  }
}
