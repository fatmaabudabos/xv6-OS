/*
Unit test for basic kthread functions: kthread_create, kthread_join, kthread_exit
Clean version for straightforward testing and log reading.
*/

#include "types.h"
#include "user.h"

// Simple thread function
void simple_thread(void *arg) {
    int thread_num = *(int*)arg;
    printf(1, "Thread %d: Started\n", thread_num);

    for (int i = 0; i < 3; i++) {
        printf(1, "Thread %d: Working... step %d\n", thread_num, i);
        sleep(5);
    }

    printf(1, "Thread %d: About to exit\n", thread_num);
    kthread_exit();
}

// Single-thread creation and join
void test_single_thread() {
    printf(1, "=== Test: Single Thread Creation ===\n");

    int arg = 1;
    int tid = kthread_create(simple_thread, &arg);

    printf(1, "Created thread with TID = %d\n", tid);
    int joined_tid = kthread_join(tid);
    printf(1, "Joined thread with TID = %d\n", joined_tid);

    printf(1, "=== Single Thread Test Complete ===\n\n");
}

// Multi-thread test with join
void test_multiple_threads() {
    printf(1, "=== Test: Multiple Thread Creation ===\n");

    int tids[3];
    int args[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++) {
        tids[i] = kthread_create(simple_thread, &args[i]);
        printf(1, "Created thread %d with TID = %d\n", i + 1, tids[i]);
    }

    for (int i = 0; i < 3; i++) {
        int joined_tid = kthread_join(tids[i]);
        printf(1, "Joined thread %d with TID = %d\n", i + 1, joined_tid);
    }

    printf(1, "=== Multiple Thread Test Complete ===\n\n");
}

int main() {
    printf(1, "Starting Basic Kernel Thread Tests\n\n");

    test_single_thread();
    test_multiple_threads();

    printf(1, "All Basic Thread Tests Completed\n");
    exit();
}
