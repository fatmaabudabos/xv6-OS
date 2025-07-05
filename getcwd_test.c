#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    char buf[64];  // buffer to hold the "cwd" string

    if (getcwd(buf, sizeof(buf)) < 0) {
        printf(2, "getcwd syscall failed\n");
        exit();
    }

    printf(1, "Current directory inode: %s\n", buf);
    exit();
}
