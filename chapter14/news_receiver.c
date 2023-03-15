#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(char *msg);

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Usage: %s <GroupIP> <port>\n", argv[0]);
  }
  int sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (sock == -1) {
    error_handling("socket() error");
  }
  struct sockaddr_in adr;

  bzero(&adr, sizeof(adr));
  adr.sin_family = AF_INET;
  adr.sin_port = htons(atoi(argv[2]));
  adr.sin_addr.s_addr = htonl(INADDR_ANY);

  struct ip_mreq join_adr;
  join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
  join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

  setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&join_adr,
             sizeof(join_adr));

  char buf[1024];
  int str_len;
  while (1) {
    // 这里接收地址为空
    str_len = recvfrom(sock, buf, 1024 - 1, 0, NULL, 0);
    if (str_len < 0) {
      break;
    }
    buf[str_len] = 0;
    puts(buf);
  }

  close(sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}