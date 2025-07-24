#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void write_line(int fd, int num) {
  char buf[16];
  int i = 0;

  // Write "Line "
  buf[i++] = 'L';
  buf[i++] = 'i';
  buf[i++] = 'n';
  buf[i++] = 'e';
  buf[i++] = ' ';
  
  // Convert number to ASCII manually
  if (num >= 100) {
    buf[i++] = '0' + num / 100;
    buf[i++] = '0' + (num / 10) % 10;
    buf[i++] = '0' + num % 10;
  } else if (num >= 10) {
    buf[i++] = '0' + (num / 10);
    buf[i++] = '0' + (num % 10);
  } else {
    buf[i++] = '0' + num;
  }

  // Add newline
  buf[i++] = '\n';

  write(fd, buf, i);
}

int main() {
  int fd = open("hundred.txt", O_CREATE | O_WRONLY);
  if (fd < 0) {
    printf(1, "Failed to create file\n");
    exit();
  }

  for (int j = 1; j <= 100; j++) {
    write_line(fd, j);
  }

  close(fd);
  exit();
}
