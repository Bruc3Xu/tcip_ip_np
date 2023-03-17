#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define NAME_SIZE 20
#define BUF_SIZE 100

void error_handling(char *msg);

void *send_msg(void *arg);
void *recv_msg(void *arg);

char name[NAME_SIZE] = "[Default]";

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("usage: %s <IP> <port> <name>\n", argv[0]);
    exit(1);
  }
  sprintf(name, "[%s]", argv[3]);

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    error_handling("socket() error");
  }
  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    error_handling("connect() error");
  }

  pthread_t th1, th2;
  pthread_create(&th1, NULL, recv_msg, (void *)&sock);
  pthread_create(&th2, NULL, send_msg, (void *)&sock);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  close(sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}

void *send_msg(void *arg) {
  int sock = *((int *)arg);

  char name_msg[NAME_SIZE + BUF_SIZE];
  char msg[BUF_SIZE];
  for (;;) {
    fgets(msg, BUF_SIZE, stdin);
    if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
      exit(0);
      close(sock);
    }
    sprintf(name_msg, "%s %s", name, msg);
    send(sock, name_msg, sizeof(name_msg), 0);
  }
  return NULL;
}
void *recv_msg(void *arg) {
  int sock = *((int *)arg);
  char name_msg[NAME_SIZE + BUF_SIZE];
  for (;;) {
    int str_len = recv(sock, name_msg, NAME_SIZE + BUF_SIZE - 1, 0);
    if (str_len == -1) {
      break;
    }
    name_msg[str_len] = 0;
    puts(name_msg);
  }
  return NULL;
}
