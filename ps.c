#include "types.h"
#include "stat.h"
#include "user.h"
#include "pinfo.h"

#define MAX 64

int main()
{
    // Initializing a list of type struct pinfo to pass it to getproclist syscall.
    struct pinfo list[MAX];
    int n = getproclist(list, MAX);

    /* The system call has 3 errors:
        1) -1 if argument 0 faces failure during fetching.
        2) -2 if argument 1 faces failure during fetching.
        3) -3 if it faces failure during copying from kernel address to user address.
    */
    if (n < 0)
    {
        printf(1, "Exited with error %d\n", n);
        exit();
    }

    // Similar to the ones declared in proc.h -> used for printing.
    char *states[] = {"UNUSED", "EMBRYO", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"};

    printf(1, "NAME\tPID\tPPID\tMEMSIZE\t  STATE\n");

    // Iterate over the list retrieved by the syscall.
    for (int i = 0; i < n; i++)
    {
        printf(1, "%s\t%d\t%d\t%d\t  %s\n", list[i].name, list[i].pid, list[i].ppid, list[i].sz, states[list[i].state]);
    }

    exit();
}