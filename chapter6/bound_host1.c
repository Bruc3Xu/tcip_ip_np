#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s <port>\n", argv[0]);
    return -1;
  }
  int server_sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (server_sock == -1) {
    error_handling("socket() error");
  }
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[1]));
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    error_handling("bind() error");
  }

  char msg[1024];

  struct sockaddr_in client_addr;
  socklen_t client_addr_len;

  int str_len = 0;

  for (int i = 0; i < 3; ++i) {
    sleep(5);
    str_len = recvfrom(server_sock, msg, 1024, 0,
                       (struct sockaddr *)&client_addr, &client_addr_len);
    printf("recv message %d: %s\n", i + 1, msg);
  }
  close(server_sock);

  return 0;
}
