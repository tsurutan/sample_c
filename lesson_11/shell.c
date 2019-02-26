#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
  pid_t pid;
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "ERROR");
    exit(1);
  }
  if (pid == 0) {
    printf("child process pid = %d\n ", getpid());
    int result = setsid();
    printf("result = %d\n", result);
    if (argc > 3 && strcmp(argv[3], "|" ) == 0){
      printf("pipe came");
    }
    execlp(argv[1], argv[1], argv[2], NULL);
    perror(argv[1]);
    exit(99);
  } else {
    int status;
    wait(&status);
    printf("parent process pid = %d\n ", getpid());
    if(WIFEXITED(status)) {
      printf("status = %d\n ", WEXITSTATUS(status));
    }
    exit(0);
  }
}
