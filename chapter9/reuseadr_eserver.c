#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error_handling(char *msg);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    error_handling("arguments not enough");
  }

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  int server_sock = socket(PF_INET, SOCK_STREAM, 0); // error -1, other fd
  if (server_sock == -1) {
    error_handling("socket() error");
  }

  int state;
  int option = 1;
  socklen_t optlen;

  optlen = sizeof(option);

  // 注释掉下面这一行会出现bind() error
  state = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option,
                     optlen);

  if (state) {
    error_handling("setsockopt() error");
  }

  bzero(&server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(atoi(argv[1]));

  if (bind(server_sock, (struct sockaddr *)(&server_addr),
           sizeof(server_addr)) == -1) { // succ = 0, error -1
    error_handling("bind() error");
  }

  if (listen(server_sock, 5) == -1) { // backlog: waiting queue and established
                                      // connection queue total size
    error_handling("listen() error");
  }

  socklen_t client_addr_size = sizeof(client_addr);

  char msg[1024];
  int str_len;

  for (;;) {
    int client_sock = accept(server_sock, (struct sockaddr *)(&client_addr),
                             &client_addr_size);
    if (client_sock == -1) {
      error_handling("accept() error");
    } else {
      printf("connection established, client address: %s",
             inet_ntoa(client_addr.sin_addr));
    }

    while ((str_len = read(client_sock, msg, 1024)) != 0) {
      write(client_sock, msg, str_len);
    }

    close(client_sock);
  }

  close(server_sock);

  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
