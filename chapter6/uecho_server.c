#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char* argv[]){
    if(argc != 2)
    {
        printf("usage: %s port", argv[0]);
        return -1;
    }
    

    return 0;
}
