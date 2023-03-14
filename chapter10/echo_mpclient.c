#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *msg);
void read_routine(int fd, char *msg);
void write_routine(int fd, char *msg);

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

  pid_t pid = fork();
  if (pid == 0) {
    write_routine(sock, msg);
  } else {
    read_routine(sock, msg);
  }
  close(sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}

void read_routine(int fd, char *buf) {
  while (1) {
    int str_len = read(fd, buf, BUF_SIZE);
    if (str_len == -1) {
      error_handling("read() error");
      return;
    }
    if (str_len == 0) {
      return;
    }
    buf[str_len] = 0;
    printf("msg from server: %s\n", buf);
  }
}

void write_routine(int fd, char *buf) {
  while (1) {
    fgets(buf, BUF_SIZE, stdin);
    if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
      shutdown(fd, SHUT_RDWR);
      return;
    }
    write(fd, buf, strlen(buf));
  }
}