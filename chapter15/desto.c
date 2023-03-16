#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC);
  FILE *fp = fdopen(fd, "w");
  fputs("Network C programming\n", fp);
  fclose(fp);
  // 不再需要关闭
  //   close(fd);
  return 0;
}