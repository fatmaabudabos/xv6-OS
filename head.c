#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define DEFAULT_LINES 10
#define BUF_SIZE 512

int main(int argc, char *argv[]) {
  int lines = DEFAULT_LINES;
  char buf[BUF_SIZE];

  if (argc < 2) {
    fprintf(2, "Usage: head filename [num_lines]\n");
    exit();
  }

  if (argc == 3) {
    lines = atoi(argv[2]);
    if (lines <= 0) {
      fprintf(2, "Invalid number of lines: %s\n", argv[2]);
      exit();
    }
  }

  int fd = open(argv[1], 0);
  if (fd < 0) {
    fprintf(2, "head: cannot open %s\n", argv[1]);
    exit();
  }

  int n, line_count = 0;
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (int i = 0; i < n && line_count < lines; i++) {
      write(1, &buf[i], 1);
      if (buf[i] == '\n') {
        line_count++;
        if (line_count >= lines) break;
      }
    }
    if (line_count >= lines)
      break;
  }

  close(fd);
  exit();
}
