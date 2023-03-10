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

  /*
   向UDP套接字注册目标地址
  */
  if (connect(client_sock, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    error_handling("connect() error");
  }

  char msg[1024];
  int str_len;
  socklen_t from_addr_len;

  for (;;) {
    fputs("input msg(q to quit): ", stdout);
    fgets(msg, sizeof(msg), stdin);
    if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
      break;
    }

    // 不能使用空的addr,会覆盖之前的注册信息
    // struct sockaddr_in null_addr;
    // sendto(client_sock, msg, strlen(msg), 0, (struct sockaddr *)&null_addr,
    //        sizeof(null_addr));

    // write(client_sock, msg, strlen(msg));
    send(client_sock, msg, strlen(msg), 0);

    // str_len = recvfrom(client_sock, msg, 1024, 0, (struct sockaddr
    // *)&from_addr,
    //                    &from_addr_len);
    // str_len = read(client_sock, msg, 1024);
    str_len = recv(client_sock, msg, 1024, 0);

    msg[str_len] = 0;
    printf("%d bytes msg from server: %s", str_len, msg);
  }
  close(client_sock);
  return 0;
}