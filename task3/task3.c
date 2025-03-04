#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 512

int main(int argc, char* argv[]) {
    int pipefd[2];

    if (pipe(pipefd) < 0) {
        fprintf(stderr, "pipe error\n");
        exit(1);
    }
    
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid == 0) {
        close(pipefd[1]);
        char buf[BUF_SIZE];
        int len;

        while ((len = read(pipefd[0], &buf, BUF_SIZE)) > 0) {
            char* str = buf;
            while (len > 0) {
                int ret = write(1, str, len);
                if (ret < 0) {
                    fprintf(stderr, "write error\n");
                    close(pipefd[1]);
                    exit(1);
                }
                len -= ret;
                str += ret;
            }
        }

        close(pipefd[0]);
        if (len < 0) {
            fprintf(stderr, "read error\n");
            exit(1);
        }
        exit(0);
    }
    else if (pid > 0) {
        close(pipefd[0]);

        for (int i = 1; i < argc; ++i) {
            char* buf = argv[i];
            int len = strlen(argv[i]);  
            while (len > 0) {
                int ret = write(pipefd[1], buf, len);
                if (ret < 0) {
                  fprintf(stderr, "write error\n");
                  close(pipefd[1]);
                  exit(1);
                }
                len -= ret;
                buf += ret;
          }
          if (write(pipefd[1], "\n", 1) < 0) {
              fprintf(stderr, "write error\n");
              close(pipefd[1]);
              exit(1);
            }
        }
        int ret = close(pipefd[1]);

        if (ret < 0) {
            fprintf(stderr, "close error\n");
            exit(1);
        }
        else {
            wait((int*) 0);
        }
        exit(0);
    }
}


