#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void timeout(int sig) {
  if (sig == SIGALRM) {
    printf("Time out!\n");
  }
  alarm(2);
}

void keycontrol(int sig) {
  if (sig == SIGINT) {
    printf("CTRL+C pressed\n");
  }
}

int main() {
  int i;
  signal(SIGALRM, timeout);
  signal(SIGINT, keycontrol);

  alarm(2);

  for (i = 0; i < 3; ++i) {
    printf("waiting...\n");

    // 发生信号时将唤醒由于调用sleep而进入休眠的进程
    // 进程一旦被唤醒就不会再进入休眠
    sleep(100);
  }
  return 0;
}