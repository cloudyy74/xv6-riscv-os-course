#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) {
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "fork error\n");
    exit(2);
  }
  else if (pid > 0) {
    printf("parent pid = %d, child pid = %d\n", getpid(), pid);
    int ret = kill(pid);
    if (ret < 0) {
      fprintf(2, "kill error\n");
      exit(1);
    }
    int status, cpid;
    cpid = wait(&status);
    printf("child pid = %d exit with status %d\n", cpid, status);
    exit(0);
  }
  else if (pid == 0) {
    sleep(100);
    exit(1);
  }
}
