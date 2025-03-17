#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h" 
#include "proc.h"
#include "procinfo.h"

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

extern struct proc proc[NPROC];
extern struct spinlock wait_lock;

uint64
sys_ps_listinfo(void)
{
  uint64 plist;
  int lim;

  argaddr(0, &plist);
  argint(1, &lim);

  int cnt = 0;
  if (plist == 0) {
    struct proc *p;
    for (p = proc; p < &proc[NPROC]; ++p) {
      acquire(&p->lock);
      if (p->state != UNUSED) {
        ++cnt;
      }
      release(&p->lock);
    }
  }
  else {
    struct procinfo info;
    struct proc *p;

    for (p = proc; p < &proc[NPROC]; ++p) {
      acquire(&p->lock);
      if (p->state == UNUSED) {
        release(&p->lock);
        continue;
      }

      if (++cnt > lim) {
        release(&p->lock);
        return -1;
      }

      info.pid = p->pid;
      safestrcpy(info.name, p->name, sizeof(info.name));
      info.state = (int)p->state;

      acquire(&wait_lock);
      info.ppid = (p->parent) ? p->parent->pid : -1;
      release(&p->lock);
      release(&wait_lock);

      if (copyout(myproc()->pagetable, plist, (char *)&info, sizeof(info)) < 0) {
        return -2;
      }

      plist += sizeof(info);
    }
  }
  return cnt;
}
