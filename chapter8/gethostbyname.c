#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error_handling(char *msg);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s <addr>\n", argv[0]);
    exit(1);
  }
  struct hostent *host;

  host = gethostbyname(argv[1]);
  if (!host) {
    error_handling("gethostbyname() error");
  }
  printf("official name: %s\n", host->h_name);
  int i;
  for (int i = 0; host->h_aliases[i]; ++i) {
    printf("alias name %d: %s\n", i + 1, host->h_aliases[i]);
  }
  if (host->h_addrtype == AF_INET) {
    printf("addr type: IPV4\n");
  } else {
    printf("addr type: IPV6\n");
  }

  for (int i = 0; host->h_addr_list[i]; ++i) {
    printf("addr %d: %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
  }

  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
