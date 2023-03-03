#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main() {
  int fd = open("data.txt", O_RDONLY);
  if (fd == -1) {
    error_handling("open() error");
  }
  char buf[30];
  if (read(fd, buf, sizeof(buf)) == -1) {
    error_handling("read() error");
  }
  printf("read data: %s", buf);
  close(fd);
  return 0;
}