#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
  fd_set reads, temps;
  FD_ZERO(&reads);
  FD_SET(0, &reads); // 0 for standard input

  struct timeval timeout;

  // 不能在此设置，select调用会修改这一结构体，每次调用前必须reset,temps同理
  /*
  timeout.tv_sec = 5;
  timeout.tv_usec = 5000;
  */

  int result, str_len;
  char buf[BUF_SIZE];

  for (;;) {
    temps = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    result = select(1, &temps, 0, 0, &timeout);
    if (result == -1) {
      puts("select() error\n");
      break;
    } else if (result == 0) {
      puts("timeout!\n");

    } else {
      if (FD_ISSET(0, &temps)) {
        str_len = read(0, buf, BUF_SIZE);
        buf[str_len] = 0;
        printf("message from console: %s\n", buf);
      }
    }
  }
  return 0;
}