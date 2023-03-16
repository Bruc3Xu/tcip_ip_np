#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void error_handling(char *msg);
void setnonblockingmode(int sock);

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

  int epfd = epoll_create(EPOLL_SIZE);
  if (epfd == -1) {
    error_handling("epoll_create() error");
  }
  struct epoll_event *ep_events =
      malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = sock;
  epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &event);
  int event_cnt;

  struct sockaddr_in client_adr;
  socklen_t client_adr_len = sizeof(client_adr);
  int client_sock;
  int str_len;
  char buf[BUF_SIZE];

  while (1) {
    event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
    if (event_cnt == -1) {
      puts("epoll_wait() error\n");
      break;
    }
    puts("return epoll_wait()\n");
    for (int i = 0; i < event_cnt; ++i) {
      if (ep_events[i].data.fd == sock) {
        client_sock =
            accept(sock, (struct sockaddr *)&client_adr, &client_adr_len);
        if (client_sock == -1) {
          puts("accept() error\n");
          continue;
        }
        setnonblockingmode(client_sock);
        event.events = EPOLLIN | EPOLLET;
        event.data.fd = client_sock;
        epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &event);
        printf("connected client: %d\n", client_sock);
      } else {
        // 这里循环读取，直到把输入缓冲中的数据全部读完
        while (1) {
          str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
          if (str_len == 0) {
            epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
            close(ep_events[i].data.fd);
            printf("closed client: %d\n", ep_events[i].data.fd);
          } else if (str_len < 0) {
            if (errno == EAGAIN) {
              break;
            }
          } else {
            write(ep_events[i].data.fd, buf, str_len);
          }
        }
      }
    }
  }
  close(epfd);
  close(sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}

void setnonblockingmode(int sock) {
  int flag = fcntl(sock, F_GETFL, 0);
  fcntl(sock, F_SETFL, flag | O_NONBLOCK);
}
