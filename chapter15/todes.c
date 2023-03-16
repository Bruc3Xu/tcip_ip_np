#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC);
  printf("first descriptor %d \n", fd);
  FILE *fp = fdopen(fd, "w");
  fputs("Network C programming\n", fp);
  printf("second descriptor %d\n", fileno(fp));
  fclose(fp);
  return 0;
}