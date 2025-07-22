#include "types.h"
#include "user.h"


sleeplock_t test_sleeplock;
int resource_in_use = 0;
int access_count = 0;

void access_resource(void *arg) {
    sleeplock_acquire(&test_sleeplock);

    if (resource_in_use != 0)
        printf(1, "ERROR: Resource conflict!\n");

    resource_in_use = 1;
    access_count++;
    sleep(10);
    resource_in_use = 0;

    sleeplock_release(&test_sleeplock);
    kthread_exit();
}

void test_sleeplock_basic() {
    sleeplock_init(&test_sleeplock);
    sleeplock_acquire(&test_sleeplock);
    sleeplock_release(&test_sleeplock);
}

void test_sleeplock_blocking() {
    sleeplock_init(&test_sleeplock);
    resource_in_use = 0;
    access_count = 0;

    int tids[3];
    int args[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++) {
        tids[i] = kthread_create(access_resource, &args[i]);
        if (tids[i] <= 0)
            printf(1, "Thread %d creation failed\n", i + 1);
        sleep(2); // small delay to stagger execution
    }

    for (int i = 0; i < 3; i++)
        kthread_join(tids[i]);
    
    printf(1, "Access count: %d (Expected: 3)\n", access_count);
}

int main() {
    test_sleeplock_basic();
    test_sleeplock_blocking();
    exit();
}
