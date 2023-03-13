#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

void read_childproc(int sig) {
  if (sig == SIGCHLD) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
      printf("remove proc %d\n", pid);
      printf("child send %d\n", WEXITSTATUS(status));
    }
  }
}

int main() {
  pid_t pid = fork();

  struct sigaction act;
  act.sa_handler = read_childproc;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  sigaction(SIGCHLD, &act, 0);

  if (pid == 0) {
    printf("Hi! I'm child process\n");
    sleep(10);
    return 12;
  } else {
    printf("child pid %d \n", pid);
    pid = fork();
    if (pid == 0) {
      printf("Hi! I'm child process\n");
      sleep(10);
      exit(24);
    } else {
      printf("child pid %d \n", pid);
      for (int i = 0; i < 5; ++i) {
        printf("wait...\n");
        sleep(5);
      }
    }
  }

  return 0;
}