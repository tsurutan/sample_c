#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#define BUF 256
int
main (int argc, char *argv[])
{
  pid_t pid;
  pid = fork();
  if (pid > 0) {
    waitpid(pid, NULL, 0);
  } else {
    FILE *fp;
    char  *cmdline = "cat";
    if ( (fp=popen(argv[1],"w")) ==NULL) {
      err(EXIT_FAILURE, "%s", cmdline);
    }

    (void) fputs("one", fp);
    (void) pclose(fp);

    exit (EXIT_SUCCESS);
  }
}
