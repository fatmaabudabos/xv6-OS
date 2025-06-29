#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

int
main(void)
{
  int i;

  // Call getpid() a few times
  for(i = 0; i < 5; i++) {
    getpid();
  }

  // Now get the syscall count for SYS_getpid (which is syscall number 11)
  int count = getsyscallcount(SYS_getpid);

  printf(1, "getpid was called %d times\n", count);

  // Try an invalid syscall number
  int invalid = getsyscallcount(999);
  printf(1, "Invalid syscall count: %d\n", invalid);

  exit();
}
