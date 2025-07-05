#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define DEFAULT_LINES 10    // Default number of lines to print
#define BUF_SIZE 512        // Buffer size for reading chunks of the file

int main(int argc, char *argv[]) {
  int lines = DEFAULT_LINES;   // Number of lines to print
  char buf[BUF_SIZE];          // Buffer for reading file content

  // ✅ Check if at least one argument is passed (the filename)
  if (argc < 2) {
    printf(1, "Usage: head filename [num_lines]\n");
    exit();
  }

  // ✅ If a second argument is passed, parse it as the number of lines to print
  if (argc == 3) {
    lines = atoi(argv[2]);
    if (lines <= 0) {
      printf(1, "Invalid number of lines: %s\n", argv[2]);
      exit();
    }
  }

  // ✅ Open the file in read-only mode
  int fd = open(argv[1], 0);
  if (fd < 0) {
    printf(1, "head: cannot open %s\n", argv[1]);
    exit();
  }

  int n, line_count = 0;

  // ✅ Read the file in chunks (BUF_SIZE) until we reach the desired number of lines
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (int i = 0; i < n; i++) {
      write(1, &buf[i], 1);          // Print the character to stdout
      if (buf[i] == '\n') {          // Count newline characters
        line_count++;
        if (line_count >= lines) {   // Stop if we've reached the required line count
          close(fd);
          exit();
        }
      }
    }
  }

  // ✅ If read returns an error
  if (n < 0) {
    printf(1, "head: read error\n");
  }

  close(fd);  // ✅ Close the file
  exit();     // ✅ Exit the program
}
