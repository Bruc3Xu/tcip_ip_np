#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error_handling(char *msg);

int main(int argc, char *argv[]) {
  int tcp_sock = socket(PF_INET, SOCK_STREAM, 0);

  int state;
  int buf_size = 1024 * 3;
  socklen_t optlen;

  optlen = sizeof(buf_size);
  state =
      setsockopt(tcp_sock, SOL_SOCKET, SO_RCVBUF, (void *)&buf_size, optlen);
  if (state) {
    error_handling("setsockopt() error");
  }
  state =
      setsockopt(tcp_sock, SOL_SOCKET, SO_SNDBUF, (void *)&buf_size, optlen);
  if (state) {
    error_handling("setsockopt() error");
  }

  state =
      getsockopt(tcp_sock, SOL_SOCKET, SO_RCVBUF, (void *)&buf_size, &optlen);
  if (state) {
    error_handling("getsockopt() error");
  }
  printf("recv buffer size: %d\n", buf_size);
  state =
      getsockopt(tcp_sock, SOL_SOCKET, SO_SNDBUF, (void *)&buf_size, &optlen);
  if (state) {
    error_handling("getsockopt() error");
  }
  printf("send buffer size: %d\n", buf_size);

  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
