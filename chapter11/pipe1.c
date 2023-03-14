#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 30

int main() {
  int fds[2];
  char str[] = "Who are you?\n";
  char buf[BUF_SIZE];
  pipe(fds);

  pid_t pid = fork();
  if (pid == -1) {
    printf("fork() error\n");
    exit(1);
  } else if (pid == 0) {
    write(fds[1], str, sizeof(str));
  } else {
    read(fds[0], buf, BUF_SIZE);
    puts(buf);
  }
  return 0;
}
