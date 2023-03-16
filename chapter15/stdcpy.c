#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 3

int main(int argc, char *argv[]) {
  FILE *fp1, *fp2;
  char buf[BUFSIZ];
  fp1 = fopen("news.txt", "r");
  fp2 = fopen("cpy.txt", "w");
  int len;
  while (fgets(buf, BUF_SIZE, fp1) != NULL) {
    fputs(buf, fp2);
  }
  fclose(fp1);
  fclose(fp2);
  return 0;
}