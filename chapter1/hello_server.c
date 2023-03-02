#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void sig_interrupt(char *msg);
void error_handling(char *msg);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    error_handling("arguments not enough");
  }

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  int server_sock = socket(PF_INET, SOCK_STREAM, 0); // error -1, other fd
  if (server_sock == -1) {
    error_handling("socker error");
  }
  memset(&server_addr, 0, sizeof(server_addr)); // initialize addr
  // bzero(&server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(atoi(argv[1]));

  if (bind(server_sock, (struct sockaddr *)(&server_addr),
           sizeof(server_addr)) == -1) { // succ = 0, error -1
    error_handling("binding error");
  }

  if (listen(server_sock, 5) == -1) { // backlog: waiting queue and established
                                      // connection queue total size
    error_handling("listen error");
  }

  for (;;) {
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock = accept(server_sock, (struct sockaddr *)(&client_addr),
                             &client_addr_size);
    if (client_sock == -1) {
      error_handling("accept error");
    }

    char recv_msg[1024];
    int nbytes = read(client_sock, recv_msg, sizeof(recv_msg) - 1);
    if (nbytes == -1) {
      error_handling("error reading");
    } else if (nbytes == 0) {
      printf("read EOF\n");
      continue;
    } else {
      printf("read %d bytes data", nbytes);
    }

    strcat(recv_msg, " with server reply\n");
    printf("reply msg is %s", recv_msg);
    if (write(client_sock, recv_msg, sizeof(recv_msg)) == -1) {
      error_handling("write msg error");
    }
    close(client_sock);
  }
  close(server_sock);

  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
