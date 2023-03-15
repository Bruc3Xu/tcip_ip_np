#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(char *msg);

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Usage: %s <IP> <port>\n", argv[0]);
  }
  int sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (sock == -1) {
    error_handling("socket() error");
  }
  struct sockaddr_in mul_adr;

  bzero(&mul_adr, sizeof(mul_adr));
  mul_adr.sin_family = AF_INET;
  mul_adr.sin_port = htons(atoi(argv[2]));
  mul_adr.sin_addr.s_addr = inet_addr(argv[1]);

  int ttl = 64;
  setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&ttl, sizeof(ttl));

  FILE *fp;
  if ((fp = fopen("news.txt", "r")) == NULL) {
    error_handling("fopen() error\n");
  }

  char buf[1024];
  while (!feof(fp)) {
    fgets(buf, 1024, fp);
    sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&mul_adr,
           sizeof(mul_adr));
    sleep(2);
  }

  fclose(fp);
  close(sock);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}