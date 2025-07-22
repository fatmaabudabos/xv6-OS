/*
Unit test for condition variable functions: cond_init, cond_wait, cond_signal
and how they coordinate thread blocking and waking using sleeplocks.
*/

#include "types.h"
#include "user.h"
#include "kthread.h"  // Updated wrapper API

// Global test state
cond_var_t test_cond;
sleeplock_t cond_lock;
int ready_flag = 0;
int signal_count = 0;

// Thread that waits on condition variable
void waiting_thread(void *arg) {
    int thread_id = *(int *)arg;

    printf(1, "Thread %d: Acquiring lock to wait on condition\n", thread_id);
    sleeplock_acquire(&cond_lock);

    printf(1, "Thread %d: Waiting for condition signal...\n", thread_id);
    while (ready_flag == 0) {
        cond_wait(&test_cond, &cond_lock);
    }

    signal_count++;
    printf(1, "Thread %d: Got signal! Ready flag = %d\n", thread_id, ready_flag);

    sleeplock_release(&cond_lock);
    printf(1, "Thread %d: Exiting\n", thread_id);
    kthread_exit();
}

// Thread that signals the condition variable
void signaling_thread(void *arg) {
    printf(1, "Signaling thread: Starting, will signal in a moment...\n");

    sleep(20); // Wait for waiting threads to block

    printf(1, "Signaling thread: Acquiring lock to set ready flag\n");
    sleeplock_acquire(&cond_lock);

    ready_flag = 1;
    printf(1, "Signaling thread: Set ready flag = 1, sending signal\n");
    cond_signal(&test_cond); // wakes only one thread

    sleeplock_release(&cond_lock);
    printf(1, "Signaling thread: Signal sent, exiting\n");
    kthread_exit();
}

// Basic initialization test
void test_condvar_basic() {
    printf(1, "=== Test: Basic Condition Variable Operations ===\n");

    cond_init(&test_cond);
    sleeplock_init(&cond_lock);
    printf(1, "Condition variable and lock initialized\n");

    printf(1, "=== Basic Condition Variable Test Complete ===\n\n");
}

// Test wait/signal behavior with competing threads
void test_condvar_signal() {
    printf(1, "=== Test: Condition Variable Wait/Signal ===\n");

    ready_flag = 0;
    signal_count = 0;
    cond_init(&test_cond);
    sleeplock_init(&cond_lock);

    int tids[3]; // 0,1 for waiters, 2 for signaler
    int waiter_args[2] = {1, 2};
    int signaler_arg = 99;

    printf(1, "Creating 2 waiting threads and 1 signaling thread...\n");

    // Create waiter threads
    for (int i = 0; i < 2; i++) {
        tids[i] = kthread_create(waiting_thread, &waiter_args[i]);
        if (tids[i] <= 0) {
            printf(1, "ERROR: Failed to create waiting thread %d\n", i + 1);
            return;
        }
    }

    // Create signaler
    tids[2] = kthread_create(signaling_thread, &signaler_arg);
    if (tids[2] <= 0) {
        printf(1, "ERROR: Failed to create signaling thread\n");
        return;
    }

    // Join signaler
    if (kthread_join(tids[2]) != 0) {
        printf(1, "ERROR: Failed to join signaling thread\n");
    }

    sleep(10); // Allow signal to propagate

    // Join waiters
    for (int i = 0; i < 2; i++) {
        if (kthread_join(tids[i]) != 0) {
            printf(1, "ERROR: Failed to join waiting thread %d\n", i + 1);
        }
    }

    printf(1, "Threads that received signal: %d\n", signal_count);
    printf(1, "Note: Depending on cond_signal vs cond_broadcast, result may vary\n");

    if (signal_count > 0) {
        printf(1, "SUCCESS: At least one thread received the signal\n");
    } else {
        printf(1, "FAILURE: No threads received the signal\n");
    }

    printf(1, "=== Condition Variable Signal Test Complete ===\n\n");
}

int main() {
    printf(1, "Starting Condition Variable Tests\n\n");

    test_condvar_basic();
    test_condvar_signal();

    printf(1, "All Condition Variable Tests Completed\n");
    exit();
}
