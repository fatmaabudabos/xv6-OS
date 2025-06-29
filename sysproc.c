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

// defining it as an extern struct since it is already defined in proc.c
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
  // Initializing a list of type struct pinfo & integer to retrieve from the user.
  struct pinfo *list;
  int max;

  // Fetching argument 0 (pointer) from the user and returning -1 if failed.
  if (argptr(0, (void *)&list, sizeof(*list)) < 0)
    return -1;
  // Fetching argument 1 (integer) from the user and returning -2 if failed.
  if (argint(1, &max) < 0)
    return -2;

  // counter for number of processes running in any state.
  int counter = 0;

  // Acquiring the lock on the process table to avoid race condition or wrong results.
  acquire(&ptable.lock);

  // Iterating over active (not UNUSED) processes.
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

      // Copying from kernel space to user space and returning -3 if failed.
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

#define NSYSCALLS 23  
extern int syscall_counts[];  // declared in syscall.c

int sys_getsyscallcount(void)
{
  int num;
  if (argint(0, &num) < 0 || num < 0 || num >= NSYSCALLS)
    return -1;
  return syscall_counts[num];
}
