#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SMALL_BUF 100
#define BUF_SIZE 1024

void *request_handler(void *arg);
void error_handling(char *msg);
void send_error(FILE *fp);
char *content_type(char *file);
void send_data(FILE *fp, char *ct, char *file_name);

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("usage: %s <port>\n", argv[0]);
    exit(1);
  }
  struct sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[1]));
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    error_handling("socket() error");
  }

  int reuse_addr = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&reuse_addr, sizeof(reuse_addr));

  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
  {
    error_handling("bind() error");
  }
  if (listen(sock, 5) == -1)
  {
    error_handling("listen() error");
  }

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_len = sizeof(clnt_addr);

  pthread_t thd;
  for (;;)
  {
    int clnt_sock = accept(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
    if (clnt_sock == -1)
    {
      continue;
    }

    pthread_create(&thd, NULL, request_handler, (void *)&clnt_sock);
    pthread_detach(thd);
    printf("conencted client IP: %s\n", inet_ntoa(clnt_addr.sin_addr));
  }
  close(sock);
  return 0;
}

void error_handling(char *msg)
{
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}

void *request_handler(void *arg)
{
  int sock = *((int *)arg);
  int str_len = 0;
  char req_line[SMALL_BUF];

  FILE *clnt_read, *clnt_write;
  clnt_read = fdopen(sock, "r");
  clnt_write = fdopen(sock, "w");

  fgets(req_line, SMALL_BUF, clnt_read);
  printf("recv request: %s\n", req_line);

  if (strstr(req_line, "HTTP/") == NULL)
  {
    send_error(clnt_write);
    fclose(clnt_read);
    fclose(clnt_write);
    return NULL;
  }

  char method[10];
  char filename[30];
  char ct[15];

  strcpy(method, strtok(req_line, " /"));
  strcpy(filename, strtok(NULL, " /"));
  strcpy(ct, content_type(filename));

  if (strcmp(method, "GET") != 0)
  {
    send_error(clnt_write);
    fclose(clnt_read);
    fclose(clnt_write);
    return NULL;
  }

  printf("parsed request: method: %s, filename: %s, ct: %s\n", method, filename, ct);

  fclose(clnt_read);
  send_data(clnt_write, ct, filename);
  return NULL;
}

void send_data(FILE *fp, char *ct, char *file_name)
{
  char protocol[] = "HTTP/1.1 200 OK\r\n";
  char server[] = "Server:Linux Web Server \r\n";
  char clnt_len[] = "Content-length:2048\r\n";
  char clnt_type[SMALL_BUF];
  char buf[BUF_SIZE];

  FILE *send_file;

  sprintf(clnt_type, "Content-type:%s\r\n\r\n", ct);
  send_file = fopen(file_name, "r");
  if (send_file == NULL)
  {
    send_error(fp);
    return;
  }
  fputs(protocol, fp);
  fputs(server, fp);
  fputs(clnt_len, fp);
  fputs(clnt_type, fp);

  while (fgets(buf, BUF_SIZE, send_file) != NULL)
  {
    printf("send buf: %s\n", buf);
    fputs(buf, fp);
    fflush(fp);
  }
  fflush(fp);
  fclose(fp);
}

char *content_type(char *file)
{
  char extension[SMALL_BUF];
  char file_name[SMALL_BUF];

  strcpy(file_name, file);
  strtok(file_name, ".");
  strcpy(extension, strtok(NULL, "."));

  if (!strcmp(extension, "htm") || !strcmp(extension, "html"))
  {
    return "text/html";
  }
  else
  {
    return "text/plain";
  }
}

void send_error(FILE *fp)
{
  char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
  char server[] = "Server:Linux Web Server \r\n";
  char clnt_len[] = "Content-length:2048\r\n";
  char clnt_type[] = "Content-type:text/html\r\n\r\n";
  char content[] = "<html><head><title>Network</title></head> <body><font size=+5><br>发生错误！查看请求文件名和请求方式</font></body></html>";
  fputs(protocol, fp);
  fputs(server, fp);
  fputs(clnt_len, fp);
  fputs(clnt_type, fp);
  fputs(content, fp);
  fflush(fp);
}