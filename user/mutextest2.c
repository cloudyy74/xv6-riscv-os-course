#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int mu;

void print_no_mutex(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        for (char* c = argv[i]; *c; c++) {
            printf("%d: arg %d, char ", getpid(), i);
            write(0, c, 1);
            printf("\n");
        }
    }
}

void print_mutex(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        for (char* c = argv[i]; *c; c++) {
            if (mutex_lock(mu) < 0) {
              fprintf(2, "mutex lock error\n");
            }
            printf("%d: arg %d, char ", getpid(), i);
            write(0, c, 1);
            printf("\n");
            if (mutex_unlock(mu) < 0) {
              fprintf(2, "mutex unlock error\n");
            }
        }
    }
}

int
main(int argc, char *argv[])
{
  printf("without mutex\n");
  int pid = fork();

  if (pid < 0) {
    fprintf(2, "fork error\n");
    exit(2);
  }

  print_no_mutex(argc, argv);

  if (pid == 0) {
    exit(0);
  }
  else if (pid > 0) {
    wait((int*) 0);
  }

  printf("with mutex\n");
  if ((mu = mutex()) < 0) {
    fprintf(2, "mutex error\n");
    exit(1);
  }

  pid = fork();

  if (pid < 0) {
    fprintf(2, "fork error\n");
    exit(2);
  }

  print_mutex(argc, argv);

  if (pid == 0) {
    exit(0);
  }
  else {
    wait((int*) 0);
  }
}
