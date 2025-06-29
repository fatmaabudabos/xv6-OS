#include "types.h"
#include "stat.h"
#include "user.h"
#include "pinfo.h"

#define MAX 64

int main()
{
    struct pinfo list[MAX];
    int n = getproclist(list, MAX);

    if (n < 0)
    {
        printf(1, "Exited with error %d\n", n);
        exit();
    }

    char *states[] = {"UNUSED", "EMBRYO", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"};

    printf(1, "NAME\tPID\tPPID\tMEMSIZE\t  STATE\n");

    for (int i = 0; i < n; i++)
    {
        printf(1, "%s\t%d\t%d\t%d\t  %s\n",
               list[i].name,
               list[i].pid,
               list[i].ppid,
               list[i].sz,
               states[list[i].state]);
    }

    exit();
}