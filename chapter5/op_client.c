#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OP_SIZE 4

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
  // server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(sock, (struct sockaddr *)(&server_addr), sizeof(server_addr)) ==
      -1) {
    // error -1
    error_handling("connect() error");
  }
  printf("connected to server...\nsockfd: %d\n", sock);
  char msg[BUF_SIZE];
  int op_cnt, result;

  printf("Operand count: ");
  scanf("%d", &op_cnt);
  msg[0] = (char)op_cnt;

  for (int i = 0; i < op_cnt; ++i) {
    printf("Operand %d: ", i + 1);
    scanf("%d", (int *)&msg[i * OP_SIZE + 1]);
  }
  fgetc(stdin);
  fputs("Operand: ", stdout);
  scanf("%c", &msg[op_cnt * OP_SIZE + 1]);

  // int send_size = write(sock, msg, op_cnt * OP_SIZE + 2);
  int send_size = send(sock, msg, op_cnt * OP_SIZE + 2, 0);
  printf("send size = %d\n", send_size);

  // int recv_cnt = read(sock, &result, 4);
  int recv_cnt = recv(sock, &result, 4, 0);
  if (recv_cnt == -1) {
    error_handling("read() error");
  }

  printf("read %d bytes data\n", recv_cnt);
  printf("result from server: %d\n", result);

  shutdown(sock, 0);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}