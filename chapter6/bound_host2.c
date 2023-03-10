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

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  char msg1[] = "Hi!", msg2[] = "I'm another UDP host!",
       msg3[] = "Nice to meet you!";

  sendto(client_sock, msg1, sizeof(msg1), 0, (struct sockaddr *)&server_addr,
         sizeof(server_addr));

  sendto(client_sock, msg2, sizeof(msg2), 0, (struct sockaddr *)&server_addr,
         sizeof(server_addr));
  sendto(client_sock, msg3, sizeof(msg3), 0, (struct sockaddr *)&server_addr,
         sizeof(server_addr));

  close(client_sock);
  return 0;
}