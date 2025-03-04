#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "sys/types.h"

int 
main(int argc, char *argv[]) {
  int pipefd[2];

  if (pipe(pipefd) < 0) {
    fprintf(2, "pipe error\n");
    exit(1);
  }

  pid_t pid = fork();

  if (pid < 0) {
    fprintf(2, "fork error\n");
    exit(1);
  }
  else if (pid == 0) {
    close(pipefd[1]);
    close(0);

    dup(pipefd[0]);
    close(pipefd[0]);

    char *argv[] = {"/wc", 0};
    exec("/wc", argv);

    fprintf(2, "exec error\n");
    exit(1);
  }
  else if (pid > 0) {
    close(pipefd[0]);

    for (int i = 1; i < argc; ++i) {
      char* buf = argv[i];
      int len = strlen(argv[i]);  
      while (len > 0) {
        int ret = write(pipefd[1], buf, len);
        if (ret < 0) {
          fprintf(2, "write error\n");
          close(pipefd[1]);
          exit(1);
        }
        len -= ret;
        buf += ret;
      }
      if (write(pipefd[1], "\n", 1) < 0) {
        fprintf(2, "write error\n");
        close(pipefd[1]);
        exit(1);
      }
    }
    int ret = close(pipefd[1]);

    if (ret < 0) {
      fprintf(2, "close error\n");
      exit(1);
    }
    else {
      wait((int*) 0);
    }
    exit(0);
  }
}
