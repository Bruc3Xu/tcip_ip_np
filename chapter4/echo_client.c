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
  // server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(sock, (struct sockaddr *)(&server_addr), sizeof(server_addr)) ==
      -1) {
    // error -1
    error_handling("connect() error");
  }
  char msg[BUF_SIZE];

  int str_len = 0, recv_size = 0, recv_cnt = 0;
  while (1) {
    fputs("input msg(q/Q to quit): ", stdout);
    fgets(msg, BUF_SIZE, stdin);
    if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
      break;
    }
    str_len = write(sock, msg, strlen(msg));  // 需要考虑数据发不完的情况

    // int recv_size = read(sock, &msg[idx++], BUF_SIZE - 1);  // 需要考虑收到多个数据的情况

    /* 使用下面的方式，可以在确定发送数据大小的情况下，完整读取服务器发回的数据*/
    while (recv_size < str_len) {
      recv_cnt = read(sock, &msg, BUF_SIZE - 1);
      if(recv_cnt == -1){
        error_handling("read() error");
      }else {
        recv_size += recv_cnt;
      }
    }
    // 更多见chapter5应用层协议设计
    /*========================================================*/

    msg[recv_size] = 0;
    printf("read %d bytes data\n", str_len);
    printf("msg from server: %s", msg);
  }

  close(sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}