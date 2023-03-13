#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main() {
  pid_t pid = fork();
  int status;
  if (pid == 0) {
    sleep(15);
    return 24;
  } else {
    printf("child pid %d \n", pid);
    while (!waitpid(-1, &status, WNOHANG)) {
      sleep(3);
      printf("sleep 3 secs\n");
    }
    if (WIFEXITED(status)) {
      printf("child process send %d \n", WEXITSTATUS(status));
    }
  }
  return 0;
}