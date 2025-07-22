/*
Simple unit test for user-level spinlock:
Tests spinlock_init, spinlock_acquire, spinlock_release
with basic locking and two threads incrementing a shared counter.
*/

#include "types.h"
#include "user.h"


// Global variables
spinlock_t test_lock;
int shared_counter = 0;
int iterations = 10;

// Thread function to safely increment a shared counter
void increment_with_spinlock(void *arg) {
    int tid = *(int*)arg;

    for (int i = 0; i < iterations; i++) {
        spinlock_acquire(&test_lock);
        shared_counter++;
        spinlock_release(&test_lock);
        sleep(2);  // simulate work
    }

    printf(1, "Thread %d: Done\n", tid);
    kthread_exit();
}

// Basic spinlock acquire/release test
void test_basic_spinlock() {
    printf(1, "=== Test: Basic Spinlock ===\n");

    spinlock_init(&test_lock);
    spinlock_acquire(&test_lock);
    spinlock_release(&test_lock);

    printf(1, "Basic spinlock acquire/release works\n");
    printf(1, "=== End Basic Spinlock Test ===\n\n");
}

// Multi-thread test to verify locking correctness
void test_spinlock_with_threads() {
    printf(1, "=== Test: Spinlock with Threads ===\n");

    shared_counter = 0;
    spinlock_init(&test_lock);

    int tids[2];
    int args[2] = {1, 2};

    tids[0] = kthread_create(increment_with_spinlock, &args[0]);
    tids[1] = kthread_create(increment_with_spinlock, &args[1]);

    kthread_join(tids[0]);
    kthread_join(tids[1]);

    printf(1, "Final counter: %d (expected %d)\n", shared_counter, 2 * iterations);

    if (shared_counter == 2 * iterations) {
        printf(1, "SUCCESS: Spinlock ensured mutual exclusion\n");
    } else {
        printf(1, "FAILURE: Race condition occurred!\n");
    }

    printf(1, "=== End Spinlock Thread Test ===\n\n");
}

int main() {
    printf(1, "Starting Spinlock Tests\n\n");

    test_basic_spinlock();
    test_spinlock_with_threads();

    printf(1, "All Spinlock Tests Completed\n");
    exit();
}
