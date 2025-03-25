#include "types.h"
#include "spinlock.h"
#include "sleeplock.h"

struct mutex {
  struct sleeplock mutexlock;
  int refcnt;
  int pid;
};
