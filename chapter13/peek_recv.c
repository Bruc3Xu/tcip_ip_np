#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

void error_handling(char *msg);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    error_handling("arguments not enough");
  }

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  int server_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (server_sock == -1) {
    error_handling("socket() error");
  }
  bzero(&server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(atoi(argv[1]));

  if (bind(server_sock, (struct sockaddr *)(&server_addr),
           sizeof(server_addr)) == -1) {
    error_handling("bind() error");
  }

  if (listen(server_sock, 5) == -1) {
    error_handling("listen() error");
  }

  socklen_t client_addr_size = sizeof(client_addr);
  int client_sock =
      accept(server_sock, (struct sockaddr *)(&client_addr), &client_addr_size);
  if (client_sock == -1) {
    error_handling("accept() error");
  }

  int str_len;
  char msg[BUF_SIZE];

  while (1) {
    str_len = recv(client_sock, msg, BUF_SIZE, MSG_PEEK | MSG_DONTWAIT);
    if (str_len > 0) {
      break;
    }
  }
  msg[str_len] = 0;
  printf("Buffering %d bytes: %s \n", str_len, msg);

  str_len = recv(client_sock, msg, sizeof(msg) - 1, 0);
  msg[str_len] = 0;
  printf("Read again: %s \n", msg);

  close(client_sock);
  close(server_sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
