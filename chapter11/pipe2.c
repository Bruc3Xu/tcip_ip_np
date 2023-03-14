
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 30

int main() {
  int fds[2];
  char str1[] = "Who are you?\n";
  char str2[] = "Thank you for your message!\n";
  char buf[BUF_SIZE];
  pipe(fds);

  pid_t pid = fork();
  if (pid == -1) {
    printf("fork() error\n");
    exit(1);
  } else if (pid == 0) {
    write(fds[1], str1, sizeof(str1));

    // sleep是为了父进程先把数据读走，否则子进程自己写入后读取，父进程读取阻塞
    sleep(2);
    read(fds[0], buf, BUF_SIZE);
    printf("child proc output: %s\n", buf);
  } else {
    read(fds[0], buf, BUF_SIZE);
    printf("parent proc output: %s\n", buf);
    write(fds[1], str2, sizeof(str2));
    sleep(3);
  }
  return 0;
}
