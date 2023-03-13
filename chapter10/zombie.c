#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();
  if (pid == 0) {
    printf("child process running\n");
  } else {
    printf("child process id %d\n", pid);
    sleep(30);
  }

  if (pid == 0) {
    printf("child process ends\n");
  } else {
    printf("parent process ends\n");
  }
  return 0;
}