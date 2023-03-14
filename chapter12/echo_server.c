#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

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
           sizeof(server_addr)) == -1) { // succ = 0, error -1
    error_handling("bind() error");
  }

  if (listen(server_sock, 5) == -1) { // backlog: waiting queue and established
                                      // connection queue total size
    error_handling("listen() error");
  }

  int client_sock;
  socklen_t client_addr_size = sizeof(client_addr);

  char msg[1024];
  int str_len;

  fd_set reads, cpy_reads;
  FD_ZERO(&reads);
  FD_SET(server_sock, &reads);
  int fd_max = server_sock;

  struct timeval timeout;

  int fd_num;

  for (;;) {
    cpy_reads = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;

    fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout);

    if (fd_num == -1) {
      printf("select() error\n");
      break;
    }
    if (fd_num == 0) {
      printf("select() timeout\n");
      continue;
    }
    printf("%d ready read events\n", fd_num);

    for (int fd = 0; fd < fd_max + 1; ++fd) {
      if (FD_ISSET(fd, &cpy_reads)) {
        if (fd == server_sock) // 在服务器socket上的读就绪事件，即新的请求连接
        {
          client_sock = accept(server_sock, (struct sockaddr *)(&client_addr),
                               &client_addr_size);
          if (client_sock == -1) {
            printf("accept() error\n");
            continue;
          } else {
            printf("connection established, client address: %s, socket: %d",
                   inet_ntoa(client_addr.sin_addr), client_sock);
            FD_SET(client_sock, &reads); // 注册新socket到读就绪事件集合中
            if (client_sock > fd_max) {
              fd_max = client_sock;
            }
          }
        } else { // 在其他socket的读就绪事件
          str_len = read(fd, msg, 1024);
          if (str_len == 0) {   // 接收数据EOF
            FD_CLR(fd, &reads); // 停止监听对应socket的读事件
            close(fd);          // 关闭socket
            printf("closed client: %d\n", fd);
          } else { // 有数据时，执行对应的业务逻辑
            write(fd, msg, str_len);
          }
        }
      }
    }
  }

  close(server_sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
