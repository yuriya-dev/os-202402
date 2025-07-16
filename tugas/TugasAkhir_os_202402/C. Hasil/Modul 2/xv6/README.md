MODUL 1

// ➕ Tambahan di sysproc.c
#include "spinlock.h"

extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

// ganti fungsi sys_getpinfo dengan
int sys_getpinfo(void) {
  struct pinfo *ptable_user;
  if (argptr(0, (char**)&ptable_user, sizeof(*ptable_user)) < 0)
    return -1;

  struct proc *p;
  int i = 0;

  acquire(&ptable.lock);
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if (p->state != UNUSED) {
      ptable_user->pid[i] = p->pid;
      ptable_user->mem[i] = p->sz;
      safestrcpy(ptable_user->name[i], p->name, sizeof(p->name));
      i++;
    }
  }
  release(&ptable.lock);

  return 0;
}

// ➕ Tambahan di sysfile.c
extern int readcount; // sebelum int sys_read(void)




MODUL 2

// ➕ Tambahan di proc.c
extern struct cpu *cpu;
extern struct proc *proc;

struct cpu *c = mycpu(); // setelah => struct proc *highest = 0;

// Kode lengkap fungsi scheduler
void
scheduler(void)
{
  struct proc *p;
  struct proc *highest = 0;
  struct cpu *c = mycpu(); // ➕

  for(;;){
    sti();
    acquire(&ptable.lock);

    highest = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;
      if(highest == 0 || p->priority < highest->priority)
        highest = p;
    }

    if(highest != 0){
      p = highest;
      c->proc = p;
      switchuvm(p);
      p->state = RUNNING;
      swtch(&c->scheduler, p->context);
      switchkvm();
      c->proc = 0;
    }

    release(&ptable.lock);
  }
}

// ➕ Tambahan di proc.h
int priority;  // nilai prioritas (0 = tertinggi, 100 = terendah) ➕ => di dalam struct proc {}

// Tambahan di ptest.c
// Menambahkan `sleep(1)` sebelum `set_priority()`