#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

void error_handling(char *msg);
void urg_handler(int sig);

int client_sock;

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
  client_sock =
      accept(server_sock, (struct sockaddr *)(&client_addr), &client_addr_size);
  if (client_sock == -1) {
    error_handling("accept() error");
  }

  // F_SETOWN设置拥有者
  // 把socket拥有者改为getpid()返回id的进程
  fcntl(client_sock, F_SETOWN, getpid());

  struct sigaction act;
  act.sa_flags = 0;
  act.sa_handler = urg_handler;
  sigemptyset(&act.sa_mask);

  sigaction(SIGURG, &act, 0);

  char msg[BUF_SIZE];
  int str_len;
  while ((str_len = recv(client_sock, msg, sizeof(msg), 0)) != 0) {
    if (str_len == -1) {
      continue;
    }
    msg[str_len] = 0;
    puts(msg);
  }
  // receive EOF
  close(client_sock);
  close(server_sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}

void urg_handler(int signo) {
  int str_len;
  char buf[BUF_SIZE];
  str_len = recv(client_sock, buf, sizeof(buf) - 1, MSG_OOB);
  buf[str_len] = 0;
  printf("Urgent message: %s\n", buf);
}