#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage: %s <IP> <port>\n", argv[0]);
    exit(1);
  }
  int client_sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (client_sock == -1) {
    error_handling("socket() error");
  }

  struct sockaddr_in server_addr, from_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  char msg[1024];
  int str_len;
  socklen_t from_addr_len;
  for (;;) {
    fputs("input msg(q to quit): ", stdout);
    fgets(msg, sizeof(msg), stdin);
    if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
      break;
    }
    sendto(client_sock, msg, sizeof(msg), 0, (struct sockaddr *)&server_addr,
           sizeof(server_addr));
    str_len = recvfrom(client_sock, msg, 1024, 0, (struct sockaddr *)&from_addr,
                       &from_addr_len);
    msg[str_len] = 0;
    printf("%d bytes msg from server: %s", str_len, msg);
  }
  close(client_sock);
  return 0;
}