#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

extern struct ptable ptable;

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

int sys_getpinfo(void)
{
  struct pstat *pstat;

  argptr(0, (void *)&pstat, sizeof(*pstat));

  if (pstat == 0)
  {
    return -1;
  }

  acquire(&ptable.lock);
  struct proc *p;
  int i;
  for (p = ptable.proc; p != &(ptable.proc[NPROC]); p++)
  {
    i = p - ptable.proc;

    /* if (p->state == UNUSED) { */
    /*     pstat->inuse[i] = 0; */
    /* } else { */
    /*     pstat->inuse[i] = 1; */
    /* } */
    pstat->inuse[i] = p->inuse;
    pstat->pid[i] = p->pid;
    pstat->ticks[i] = p->ticks;
    pstat->tickets[i] = p->tickets;
  }
  release(&ptable.lock);
  return 0;
}

int sys_getpid(void)
{
  return proc->pid;
}

uintp sys_sbrk(void)
{
  uintp addr;
  uintp n;

  if (arguintp(0, &n) < 0)
    return -1;
  addr = proc->sz;
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
    if (proc->killed)
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

// System call to return your favorite number
int sys_getfavnum(void)
{
  return 7;
}

// System call to shut down the system (QEMU/Bochs APM/ACPI method)
int sys_halt(void)
{
  outw(0x604, 0x2000); // QEMU/Bochs APM/ACPI shutdown
  return 0;
}

#define MAX_SYSCALL 35
extern int syscall_counts[];

// System call to return the execution count of another system call
int sys_getcount(void)
{
  int syscall_num;

  if (argint(0, &syscall_num) < 0)
    return -1;

  if (syscall_num <= 0 || syscall_num >= MAX_SYSCALL)
  {
    return -1;
  }

  return syscall_counts[syscall_num];
}

// The following functions define a portable implementation of rand and srand.

static unsigned long int next = 1; // NB: "unsigned long int" is assumed to be 32 bits wide

int rand(unsigned long int max) // RAND_MAX assumed to be 32767
{
  if (max <= 0)
    max = 32767;
  next = next * 1103515245 + 12345;
  return (unsigned int)(next / 65536) % max;
}

void srand(unsigned int seed)
{
  next = seed;
}

// System call to terminate a random process.
int sys_killrandom(void)
{
  struct proc *p;
  int process_pids[NPROC];
  int num_active = 0;
  int target_index;
  int pid_to_kill = -1;

  acquire(&ptable.lock);

  if (next == 1)
  {
    acquire(&tickslock);
    srand(ticks);
    release(&tickslock);
  }

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->state != UNUSED && p != proc && p->pid > 1)
    {
      process_pids[num_active++] = p->pid;
    }
  }

  if (num_active == 0)
  {
    release(&ptable.lock);
    return -1;
  }

  target_index = rand(0) % num_active;
  pid_to_kill = process_pids[target_index];

  release(&ptable.lock);

  if (pid_to_kill > 0 && kill(pid_to_kill) == 0)
  {

    return pid_to_kill;
  }

  return -1;
}