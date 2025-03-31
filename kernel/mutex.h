#include "types.h"
#include "spinlock.h"
#include "sleeplock.h"

struct mutex {
  struct sleeplock mutexlock;
  struct spinlock lock;
  int pid;
};
