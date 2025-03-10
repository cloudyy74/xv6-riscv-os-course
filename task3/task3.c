#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 512

int main(int argc, char* argv[]) {
    int pipefd[2];

    if (pipe(pipefd) < 0) {
        perror("pipe error\n");
        exit(1);
    }
    
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork error\n");
        exit(1);
    }
    else if (pid == 0) {
        if (close(pipefd[1]) < 0) {
            perror("close pipe write end error\n");
            exit(1);
        }
        char buf[BUF_SIZE];
        size_t len;

        while ((len = read(pipefd[0], &buf, BUF_SIZE)) > 0) {
            char* str = buf;
            while (len > 0) {
                size_t ret = write(1, str, len);
                if (ret < 0) {
                    perror("write error\n");
                    exit(1);
                }
                len -= ret;
                str += ret;
            }
        }

        if (close(pipefd[0]) < 0) {
            perror("close pipe read end error\n");
            exit(1);
        }

        if (len < 0) {
            perror("read error\n");
            exit(1);
        }
        exit(0);
    }
    else if (pid > 0) {
        if (close(pipefd[0]) < 0) {
            perror("close pipe read end error\n");
            exit(1);
        }

        for (int i = 1; i < argc; ++i) {
            char* buf = argv[i];
            size_t len = strlen(argv[i]);  
            while (len > 0) {
                size_t ret = write(pipefd[1], buf, len);
                if (ret < 0) {
                  perror("write error\n");
                  if (close(pipefd[1]) < 0) {
                      perror("close pipe write end error\n");
                  }
                  exit(1);
                }
                len -= ret;
                buf += ret;
          }
          if (write(pipefd[1], "\n", 1) < 0) {
              perror("write error\n");
              if (close(pipefd[1]) < 0) {
                  perror("close pipe write end error\n");
              }
              exit(1);
            }
        }

        if (close(pipefd[1]) < 0) {
            perror("close pipe write end error\n");
            exit(1);
        }
        else {
            wait((int*) 0);
        }
        exit(0);
    }
}


