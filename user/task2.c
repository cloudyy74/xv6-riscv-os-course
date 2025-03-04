#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "sys/types.h"

int 
main(int argc, char *argv[]) {
  int pipefd[2];
  pid_t pid;

  if (pipe(pipefd) < 0) {
    fprintf(2, "pipe error\n");
    exit(1);
  }

  pid = fork();

  if (pid < 0) {

  }
  else if (pid == 0) {
    close(0);
    dup(pipefd[0]);

    close(pipefd[0]);
    char *argv[] = {"/wc", 0};
    exec("/wc", argv);
  }
  else if (pid > 0) {

  }
}
