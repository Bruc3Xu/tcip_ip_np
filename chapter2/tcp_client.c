#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void error_handling(char *msg);

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Usage: %s <IP> <port>\n", argv[0]);
  }
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    error_handling("socket() error");
  }
  struct sockaddr_in server_addr;
  // memset(&server_addr, 0, sizeof(server_addr));
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));

  inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
  // server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(sock, (struct sockaddr *)(&server_addr), sizeof(server_addr)) ==
      -1) {
    // error -1
    error_handling("connect() error");
  }
  char msg[30];

  int str_len = 0, recv_size = 0, idx = 0;
  while (recv_size = read(sock, &msg[idx++], 1)) {
    if (recv_size == -1) {
      error_handling("read() error");
    }
    str_len += recv_size;
  }
  printf("read %d bytes data\n", str_len);
  printf("msg from server: %s", msg);
  close(sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}