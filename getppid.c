#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    int pid = getpid();
    int ppid = getppid();
    printf(1, "My PID: %d\n", pid);
    printf(1, "My Parent PID: %d\n", ppid);
    exit();
}
