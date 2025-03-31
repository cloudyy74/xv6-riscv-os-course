#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "mutex.h"
#include "param.h"
#include "proc.h"
#include "fs.h"
#include "file.h"

int 
mutexalloc(struct file **f) {
  struct mutex *mu;

  mu = 0;
  *f = 0;

  if((*f = filealloc()) == 0)
    goto bad;
  if((mu = (struct mutex*)kalloc()) == 0)
    goto bad;
  mu->pid = 0;
  initsleeplock(&mu->mutexlock, "mutex");
  initlock(&mu->lock, "mutex");
  (*f)->type = FD_MUTEX;
  (*f)->readable = 0;
  (*f)->writable = 0;
  (*f)->pipe = 0;
  (*f)->mutex = mu;
  printf("kernel: mutex %p alloced\n", mu);
  return 0;

 bad:
  if(mu)
    kfree((char*)mu);
  if(*f)
    fileclose(*f);
  return -1;
}

void 
mutexclose(struct mutex* mu) {
  kfree((char*) mu);
  printf("kernel: mutex %p closed\n", mu);
}
