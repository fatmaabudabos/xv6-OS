#include "pinfo.h"
struct stat;
struct rtcdate;

// system calls
//Declare the Function in
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int getsyscallcount(int);
int getproclist(struct pinfo *list, int max);
int getppid(void);
int clone(void (*fn)(void *), void *arg, void *stack);
int join(int tid);


// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);

// kthread library
int kthread_create(void (*fnc)(void *), void *arg);
int kthread_join(int tid);
void kthread_exit(void);

typedef struct {
    int locked;
} spinlock_t;
void spinlock_init(spinlock_t *lk);
void spinlock_acquire(spinlock_t *lk);
void spinlock_release(spinlock_t *lk);
