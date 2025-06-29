#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "pinfo.h"
#include "spinlock.h"

extern struct
{
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_getproclist(void)
{
  struct pinfo *list;
  int max;
  if (argptr(0, (void *)&list, sizeof(*list)) < 0)
    return -1;
  if (argint(1, &max) < 0)
    return -2;
  int counter = 0;
  acquire(&ptable.lock);
  for (struct proc *p = ptable.proc; p < &ptable.proc[NPROC] && counter < max; p++)
  {
    if (p->state != UNUSED)
    {
      struct pinfo process;
      process.pid = p->pid;
      process.ppid = p->parent ? p->parent->pid : 0;
      process.state = p->state;
      process.sz = p->sz;
      safestrcpy(process.name, p->name, sizeof(p->name));
      if (copyout(myproc()->pgdir, (uint)((void *)list + counter * sizeof(struct pinfo)), (char *)&process, sizeof(process)) < 0)
      {
        release(&ptable.lock);
        return -3;
      }
      counter++;
    }
  }
  release(&ptable.lock);
  return counter;
}