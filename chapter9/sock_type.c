#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error_handling(char *msg);

int main(int argc, char *argv[]) {
  int tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
  int udp_sock = socket(PF_INET, SOCK_DGRAM, 0);

  printf("tcp sock: %d, udp sock %d\n", tcp_sock, udp_sock);
  printf("SOCK_STREAM: %d, SOCK_DGRAM %d\n", SOCK_STREAM, SOCK_DGRAM);
  int state;
  int socktype;
  socklen_t optlen = sizeof(socktype);
  state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&socktype, &optlen);
  if (state) {
    error_handling("getsockopt() error");
  }
  printf("sock type one: %d\n", socktype);
  state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&socktype, &optlen);
  if (state) {
    error_handling("getsockopt() error");
  }
  printf("sock type two: %d\n", socktype);

  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
