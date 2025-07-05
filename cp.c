#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h" // for O_RDONLY, ...

// common buffer for read / write.
char buf[512];

int
main(int argc, char *argv[])
{
  // 2 file descriptors for read (fd1) & write (fd2).
  int fd1, fd2, n;

  // need 3 arguments cp <source> <destination>.
  if(argc != 3){
    printf(1, "Need 2 Arguments!\n");
    exit();
  }

  // try to open source file in fd1.
  if ((fd1 = open(argv[1], O_RDONLY)) < 0)
  {
    printf(1, "cp: cannot open %s\n", argv[1]);
    exit();
  }

  // try to open destination file in fd2.
  if((fd2 = open(argv[2], O_CREATE | O_RDWR)) < 0){
    printf(1, "cp: cannot open %s\n", argv[2]);
    exit();
  }

  // copy content of source file (fd1) to destination file (fd2).
  while((n = read(fd1, buf, sizeof(buf))) > 0) {
    write(fd2, buf, n);
  }

  close(fd1);
  close(fd2);
  
  exit();
}
