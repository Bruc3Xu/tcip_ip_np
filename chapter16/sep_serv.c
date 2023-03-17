#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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
           sizeof(server_addr)) == -1) {
    error_handling("bind() error");
  }

  if (listen(server_sock, 5) == -1) {
    error_handling("listen() error");
  }

  socklen_t client_addr_size = sizeof(client_addr);

  char msg[1024];
  int str_len;

  FILE *readfp, *writefp;

  int client_sock =
      accept(server_sock, (struct sockaddr *)(&client_addr), &client_addr_size);
  if (client_sock == -1) {
    error_handling("accept() error");
  } else {
    printf("connection established, client address: %s",
           inet_ntoa(client_addr.sin_addr));
  }

  readfp = fdopen(client_sock, "r");
  writefp = fdopen(dup(client_sock), "w");

  while (!feof(readfp)) {
    fgets(msg, 1024, readfp);
    fputs(msg, writefp);
    // fflush()会强迫将缓冲区内的数据写回参数stream指定的文件中
    fflush(writefp);
  }

  // 只关闭写
  shutdown(fileno(writefp), SHUT_WR);
  fclose(writefp);

  fgets(msg, 1024, readfp);
  fputs(msg, stdout);
  fclose(readfp);
  close(server_sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}