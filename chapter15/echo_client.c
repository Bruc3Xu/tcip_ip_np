#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>

#define BUF_SIZE 1024

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
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));

  inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

  if (connect(sock, (struct sockaddr *)(&server_addr), sizeof(server_addr)) ==
      -1) {
    error_handling("connect() error");
  }
  FILE *readfp, *writefp;
  readfp = fdopen(sock,  "r");
  writefp = fdopen(sock, "w");

  char msg[BUF_SIZE];


  int str_len = 0, recv_size = 0, recv_cnt = 0;
  while (1) {
    fputs("input msg(q/Q to quit): ", stdout);
    fgets(msg, BUF_SIZE, stdin);
    if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
      break;
    }
    fputs(msg, writefp);
    fflush(writefp);
    fgets(msg, BUF_SIZE, readfp);
    printf("msg from server: %s", msg);
  }

  fclose(readfp);
  fclose(writefp);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}