#ifndef _KTHREAD_H_
#define _KTHREAD_H_

// THREADS
int kthread_create(void (*fnc)(void *), void *arg);
int kthread_join(int tid);
void kthread_exit(void);

// SPINLOCKS
typedef struct {
    int locked;
} spinlock_t;
void spinlock_init(spinlock_t *lk);
void spinlock_acquire(spinlock_t *lk);
void spinlock_release(spinlock_t *lk);

// SLEEPLOCKS
typedef struct {
    int locked;
} sleeplock_t;
void sleeplock_init(sleeplock_t *lk);
void sleeplock_acquire(sleeplock_t *lk);
void sleeplock_release(sleeplock_t *lk);

// CONDITION VARIABLES
typedef struct {
  int dummy;  
} cond_var_t;
void cond_init(cond_var_t *cv);
void cond_wait(cond_var_t *cv, sleeplock_t *lk);
void cond_signal(cond_var_t *cv);

#endif
