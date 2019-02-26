#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

struct cmd {
    char **argv;
    long argc;      /* used length of argv */
    long capa;      /* allocated length of argv */
    int next_type;
};

struct pcmd {
  struct cmd **cmds;
  int cmdc;
};

static void invoke_cmd(struct cmd *cmd);
static char* invoke_output(struct cmd *cmd);
static struct pcmd* read_cmd(void);
static struct pcmd* parse_cmd(char *cmdline);
static void free_cmd(struct pcmd *p);
static void* xmalloc(size_t sz);
static void* xrealloc(void *ptr, size_t sz);
static char *concat_command(struct cmd *cmd);

static char *program_name;

#define PROMPT "$ "
#define PIPE 1

int
main(int argc, char *argv[])
{
    program_name = argv[0];
    for (;;) {
        struct pcmd *pcmd;
        char *output = (char*) malloc(sizeof(char) * 4048);

        fprintf(stdout, PROMPT);
        fflush(stdout);
        pcmd = read_cmd();
        for(int i = 0; i < pcmd->cmdc; i++) {
          invoke_output(pcmd->cmds[i]);
        }
        free_cmd(pcmd);
    }
    exit(0);
}

static char *invoke_output(struct cmd *cmd) {
    pid_t pid;
    int fds[2];
    pipe(fds);
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid > 0) {  /* parent */
        waitpid(pid, NULL, 0);
        return "test";
    }
    else {          /* child */
      FILE *fp;
      if((fp = popen(concat_command(cmd), "r")) == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
      }
      pclose(fp);
      exit(0);
    }
}

static char *concat_command(struct cmd *cmd) {
  char *output = (char*) malloc(sizeof(char) * 1000);
  for(int i = 0; i < cmd->argc; i++) {
    strcat(output, cmd->argv[i]);
    strcat(output, " ");
  }
  return output;
}

static void
invoke_cmd(struct cmd *cmd)
{
    pid_t pid;
    int fds[2];
    pipe(fds);
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid > 0) {  /* parent */
        waitpid(pid, NULL, 0);
    }
    else {          /* child */
      FILE *fp;
      if((fp = popen(cmd->argv[0], "w")) == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
      }
      fputs(cmd->argv[1], fp);
      pclose(fp);
      exit(0);
        /* #<{(| error |)}># */
        /* fprintf(stderr, "%s: command not found: %s\n", */
        /*                 program_name, cmd->argv[0]); */
        /* exit(1); */
    }
}

#define LINE_BUF_SIZE 2048

static struct pcmd *read_cmd(void) {
    static char buf[LINE_BUF_SIZE];
    if (fgets(buf, LINE_BUF_SIZE, stdin) == NULL) {
        exit(0);    /* allow exit by Ctrl-D (EOF) */
    }
    return parse_cmd(buf);
}

#define INIT_CAPA 16

static struct pcmd *parse_cmd(char *cmdline) {
    char *p = cmdline;
    struct cmd *cmd;
    struct pcmd *pcmd;

    pcmd = xmalloc(sizeof(struct pcmd));
    pcmd->cmdc = 0;
    pcmd->cmds = xmalloc(sizeof(struct cmd*));
    while (*p) {
      cmd = xmalloc(sizeof(struct cmd));
      cmd->argc = 0;
      cmd->argv = xmalloc(sizeof(char*) * INIT_CAPA);
      cmd->capa = INIT_CAPA;
      while(*p && *p != '|') {
        while (*p && isspace((int)*p)) {
          *p++ = '\0';
        }
        if (*p) {
          if (cmd->capa <= cmd->argc + 1) {   /* +1 for final NULL */
            cmd->capa *= 2;
            cmd->argv = xrealloc(cmd->argv, cmd->capa);
          }
          cmd->argv[cmd->argc] = p;
          cmd->argc++;
        }
        while (*p && !isspace((int)*p)) {
          p++;
        }
        if(*p == '|') {
          cmd->next_type = PIPE;
        }
      }
      cmd->argv[cmd->argc] = NULL;
      pcmd->cmds[pcmd->cmdc] = cmd;
      pcmd->cmdc++;
    }
    return pcmd;
}

static void
free_cmd(struct pcmd *pcmd)
{
    /* free(pcmd->cmd->argv); */
    free(pcmd->cmds);
    free(pcmd);
}

static void*
xmalloc(size_t size)
{
    void *p;

    p = malloc(size);
    if (!p) {
        perror("malloc");
        exit(1);
    }
    return p;
}

static void*
xrealloc(void *ptr, size_t size)
{
    void *p;

    if (!ptr) return xmalloc(size);
    p = realloc(ptr, size);
    if (!p) {
        perror("realloc");
        exit(1);
    }
    return p;
}
