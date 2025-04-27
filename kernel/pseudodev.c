#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"

struct spinlock nullstat_lock;
uint64 nullstat_count = 0;

struct spinlock urandom_lock;
uint64 urandom_state = 1;

static char zeroes[PGSIZE] = {0};
    
uint8
urandom_next_byte(void)
{
  acquire(&urandom_lock);
  urandom_state = urandom_state * 6364136223846793005ULL + 1ULL;
  uint8 result = (uint8)(urandom_state >> 56);
  release(&urandom_lock);
  return result;
}

int
pseudodev_read(short minor, int user_dst, uint64 dst, int n) 
{
  int i;
  char b;
  uint64 val;

  switch (minor) {
  case DEVNULL:
    return 0;

  case DEVZERO:
    int len = n;
    int offset = 0;
    while (len > 0)
    {
      int chunk = len > PGSIZE ? PGSIZE : len;
      if (either_copyout(user_dst, dst + offset, zeroes, chunk) < 0)
        return n - len;
      len -= chunk;
      offset += chunk;
    }
    return n;


  case DEVURANDOM:
    for (i = 0; i < n; i++) {
      b = urandom_next_byte();
      if (either_copyout(user_dst, dst + i, &b, 1) < 0)
        break;
    }
    return i;

  case DEVNULLSTAT:
    if (n != sizeof(uint64))
      return -1;
    acquire(&nullstat_lock);
    val = nullstat_count;
    release(&nullstat_lock);
    if (either_copyout(user_dst, dst, &val, sizeof(val)) < 0)
      return -1;
    return sizeof(uint64);

  default:
    return -1;
  }
}

int
pseudodev_write(short minor, int user_src, uint64 src, int n)
{
  uint64 new_seed;

  switch (minor) {
  case DEVNULL:
    return n;

  case DEVZERO:
    return -1;

  case DEVURANDOM:
    if (n != sizeof(uint64))
      return -1;
    if (either_copyin(&new_seed, user_src, src, sizeof(uint64)) < 0)
      return -1;
    acquire(&urandom_lock);
    urandom_state = new_seed;
    release(&urandom_lock);
    return sizeof(uint64);

  case DEVNULLSTAT:
    acquire(&nullstat_lock);
    nullstat_count += n;
    release(&nullstat_lock);
    return n;

  default:
    return -1;
  }
}

void
pseudodev_init(void) 
{
  initlock(&nullstat_lock, "nullstat");
  initlock(&urandom_lock, "urandom");
  devsw[PSEUDO].read  = pseudodev_read;
  devsw[PSEUDO].write = pseudodev_write;
}

