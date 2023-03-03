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
  int fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
  if (fd == -1) {
    error_handling("open() error");
  }
  char buf[] = "data to be writed\n";
  if (write(fd, buf, sizeof(buf)) == -1) {
    error_handling("write() error");
  }
  close(fd);
  return 0;
}