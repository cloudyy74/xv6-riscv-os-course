#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
  int mu;
  if ((mu = mutex()) < 0) {
    fprintf(2, "mutex error\n");
    exit(1);
  }

  printf("Test 1: read/write\n");
  char t[1];
  
  printf(read(mu, t, 1) == -1 ? "Test read: passed\n" : "Test read: failed\n");
  printf(write(mu, "smth", 4) == -1 ? "Test write: passed\n" : "Test write: failed\n");

  printf("\nTest 2: close\n");

  if (mutex_lock(mu) < 0) {
    fprintf(2, "mutex lock failed\n");
    exit(1);
  }

  printf(close(mu) == 0 ? "Test (a): passed\n" : "Test (a): failed\n");

  if ((mu = mutex()) < 0) {
    fprintf(2, "mutex error\n");
    exit(1);
  }

  if (mutex_lock(mu) < 0) {
    fprintf(2, "mutex lock failed\n");
    exit(1);
  }

  int pid = fork();

  if (pid < 0) {
    fprintf(2, "fork error\n");
    exit(1);
  }
  else if (pid == 0) {
    printf(close(mu) == 0 ? "Test (b): passed\n" : "Test (b): failed\n");
    exit(0);
  }
  else if (pid > 0) {
    wait((int*) 0);
  }
  close(mu);

  printf("\nTest 3: exit process with unclosed mutexes\n");

  pid = fork();
  
  if (pid < 0) {
    fprintf(2, "fork error\n");
    exit(1);
  }
  else if (pid == 0) {
    int mu1, mu2;
    if ((mu1 = mutex()) < 0) {
      fprintf(2, "mutex error\n");
      exit(1);
    }
    if ((mu2 = mutex()) < 0) {
      fprintf(2, "mutex error\n");
      exit(1);
    }
    if (mutex_lock(mu2) < 0) {
      fprintf(2, "mutex lock error\n");
      exit(1);
    }
    exit(0);
  }
  else if (pid > 0) {
    int status;
    wait(&status);
    printf(status == 0 ? "passed\n" : "failed\n");
  }

  printf("\nTest 4: unlock locked mutex\n");

  if ((mu = mutex()) < 0) {
    fprintf(2, "mutex error\n");
    exit(1);
  }

  if (mutex_lock(mu) < 0) {
    fprintf(2, "mutex lock error\n");
    exit(1);
  }

  pid = fork();

  if (pid < 0) {
    fprintf(2, "fork error\n");
    exit(1);
  }
  else if (pid == 0) {
    printf(mutex_unlock(mu) < 0 ? "passed\n" : "failed\n");
    exit(0);
  }
  else if (pid > 0) {
    wait((int*) 0);
  }
  exit(0);
}

