#include "types.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf(2, "Usage: touch filename\n");
  exit();
  }

  for (int i = 1; i < argc; i++) {
    int fd = open(argv[i], O_CREATE | O_RDWR);
    if (fd < 0) {
      printf(2, "touch: cannot create or open %s\n", argv[i]);
      continue;
    }
    close(fd);
  }

  exit();
}
