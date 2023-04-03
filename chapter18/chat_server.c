#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLNTS 256
#define BUF_SIZE 100

void *request_handler(void *arg);
void error_handling(char *msg);
void send_msg(char *msg, int size);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNTS];
pthread_mutex_t mutex;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s <port>\n", argv[0]);
    exit(1);
  }
  struct sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[1]));
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    error_handling("socket() error");
  }
  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    error_handling("bind() error");
  }
  if (listen(sock, 5) == -1) {
    error_handling("listen() error");
  }

  pthread_mutex_init(&mutex, NULL);

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_len = sizeof(clnt_addr);

  pthread_t thd;
  for (;;) {
    int clnt_sock = accept(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
    if (clnt_sock == -1) {
      continue;
    }
    pthread_mutex_lock(&mutex);
    clnt_socks[clnt_cnt++] = clnt_sock;
    pthread_mutex_unlock(&mutex);

    pthread_create(&thd, NULL, request_handler, (void *)&clnt_sock);
    pthread_detach(thd);
    printf("conencted client IP: %s\n", inet_ntoa(clnt_addr.sin_addr));
  }
  close(sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}

void *request_handler(void *arg) {
  int sock = *((int *)arg);
  int str_len = 0;
  char msg[BUF_SIZE];
  while ((str_len = recv(sock, msg, BUF_SIZE, 0)) != 0) {
    send_msg(msg, str_len);
  }
  pthread_mutex_lock(&mutex);

  for (int i = 0; i < clnt_cnt; ++i) {
    if (clnt_socks[i] == sock) {
      while (i++ < clnt_cnt - 1) {
        clnt_socks[i] = clnt_socks[i + 1];
      }
      break;
    }
  }
  clnt_cnt--;

  pthread_mutex_unlock(&mutex);
  close(sock);
  return NULL;
}

void send_msg(char *msg, int size) {
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < clnt_cnt; ++i) {
    send(clnt_socks[i], msg, size, 0);
  }
  pthread_mutex_unlock(&mutex);
}