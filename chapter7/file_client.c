#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

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

  char msg[BUF_SIZE];
  int recv_cnt = 0;

  FILE *fp = fopen("receive.dat", "wb");
  while ((recv_cnt = read(sock, msg, BUF_SIZE) != 0)) {
    fwrite((void *)msg, 1, recv_cnt, fp);
  }
  puts("received file data\n");
  write(sock, "Thank you", 10);
  fclose(fp);
  close(sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}