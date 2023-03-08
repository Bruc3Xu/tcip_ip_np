#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int calculate(int op_cnt, int nums[], char op);
void error_handling(char *msg);

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

  if (listen(server_sock, 15) == -1) { // backlog: waiting queue and established
                                      // connection queue total size
    error_handling("listen() error");
  }

  socklen_t client_addr_size = sizeof(client_addr);

  char msg[1024];
  int op_cnt=0, recv_cnt, recv_len;

  for (;;) {
    printf("wait for connection...\n");
    int client_sock = accept(server_sock, (struct sockaddr *)(&client_addr),
                             &client_addr_size);
    if (client_sock == -1) {
      error_handling("accept() error");
    } else {
      printf("connection established, client address: %s\n",
             inet_ntoa(client_addr.sin_addr));
    }

    // read op_cnt
    read(client_sock, &op_cnt, 1);
    printf("op_cnt: %d\n", op_cnt);

    recv_len = 0;

    while (op_cnt * 4 + 1 > recv_len) {
      recv_cnt = read(client_sock, &msg[recv_len], 1023);
      recv_len += recv_cnt;
    }

    int result = calculate(op_cnt, (int *)msg, msg[recv_len - 1]);

    write(client_sock, (char *)&result, sizeof(result));

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

int calculate(int op_cnt, int nums[], char op) {
  int result = nums[0];
  switch (op) {
  case '+':
    for (int i = 1; i < op_cnt; ++i) {
      result += nums[i];
    }
    break;
  case '-':
    for (int i = 1; i < op_cnt; ++i) {
      result -= nums[i];
    }
    break;
  case '*':
    for (int i = 1; i < op_cnt; ++i) {
      result *= nums[i];
    }
    break;
  default:
    break;
  }
  return result;
}