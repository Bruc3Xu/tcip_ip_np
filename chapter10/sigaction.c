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
  struct sigaction act;
  act.sa_handler = timeout;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  signal(SIGALRM, timeout);

  act.sa_handler = keycontrol;
  signal(SIGINT, keycontrol);

  alarm(2);

  for (i = 0; i < 3; ++i) {
    printf("waiting...\n");
    sleep(100);
  }
  return 0;
}