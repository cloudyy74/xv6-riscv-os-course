#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}



uint64 sys_pages(void) {
  uint64 buf;
  int len, flags;
  
  argaddr(0, &buf);
  argint(1, &len);
  argint(2, &flags);

  if(flags < 0 || flags > 3)
      return -1;

  struct proc *p = myproc();

  uint64 start_page = 0;
  uint64 end_page = 0;

  if (buf != 0 || len != 0) {
    start_page = PGROUNDDOWN(buf);
    end_page = PGROUNDUP(buf + len);
  }

  printf("PAGETABLE %p\n", p->pagetable);

  int mask = 0;
  if (flags & 1) mask |= PTE_D;
  if (flags & 2) mask |= PTE_A;

  vmprint(p->pagetable, start_page, end_page, mask);
  return 0;
}

uint64 sys_rmflags(void) {
  uint64 buf;
  int len, mask_user;
  
  argaddr(0, &buf);
  argint(1, &len);
  argint(2, &mask_user);

  if(mask_user < 0 || mask_user > 3)
      return -1;

  struct proc *p = myproc();

  uint64 start_page = 0;
  uint64 end_page = 0;

  if (buf != 0 || len != 0) {
    start_page = PGROUNDDOWN(buf);
    end_page = PGROUNDUP(buf + len);
  }


  int mask = 0;
  if (mask_user & 1) mask |= PTE_D;
  if (mask_user & 2) mask |= PTE_A;
  vmrmflags(p->pagetable, start_page, end_page, mask);
  return 0;
}
