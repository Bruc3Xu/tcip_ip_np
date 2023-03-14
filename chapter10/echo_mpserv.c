#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>

void error_handling(char *msg);
void read_childproc(int sig);

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
  // memset(&server_addr, 0, sizeof(server_addr)); // initialize addr
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

  socklen_t client_addr_size = sizeof(client_addr);

  char msg[1024];
  int str_len;
  pid_t pid;


  struct sigaction act;
  act.sa_handler = read_childproc;
  act.sa_flags =0;
  sigemptyset(&act.sa_mask);
  sigaction(SIGCHLD, &act, 0);

  for (;;) {
    int client_sock = accept(server_sock, (struct sockaddr *)(&client_addr),
                             &client_addr_size);
    if (client_sock == -1) {
      // 这里继续等待下一個连接
      continue;
    } else {
      printf("connection established, client address: %s:%d\n",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    pid = fork();

    if (pid == -1) {
      close(client_sock);
    } else if (pid == 0) {
      // 父进程的所有socket都会复制到子进程，这里server_sock的引用数为2,close会减1
      close(server_sock);
      while ((str_len = read(client_sock, msg, 1024)) != 0) {
        write(client_sock, msg, str_len);
      }
      printf("client disconnected...\n");
      close(client_sock);
      return 0;

    } else {
      // 同理，父进程也需要关闭client_sock
      close(client_sock);
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

void read_childproc(int sig) {
  if (sig == SIGCHLD) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
      printf("remove proc %d\n", pid);
      printf("child send %d\n", WEXITSTATUS(status));
    }
  }
}