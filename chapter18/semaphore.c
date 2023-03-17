#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem1;
sem_t sem2;
int num, sum = 0;

void *readn(void *arg);
void *accu(void *arg);

int main(int argc, char *argv[]) {
  sem_init(&sem1, 0, 0);
  sem_init(&sem2, 0, 1);
  pthread_t th1, th2;

  pthread_create(&th1, NULL, readn, NULL);
  pthread_create(&th2, NULL, accu, NULL);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);

  sem_destroy(&sem1);
  sem_destroy(&sem2);
  printf("result is %d\n", sum);
  return 0;
}

void *readn(void *arg) {
  for (int i = 0; i < 5; ++i) {
    fputs("input num: ", stdout);
    // sem2初始值为1,可以先执行，从终端取值
    // 下一次就会阻塞，等待sem_post
    sem_wait(&sem2);
    scanf("%d", &num);
    sem_post(&sem1);
  }
  return NULL;
}

void *accu(void *arg) {
  for (int i = 0; i < 5; ++i) {
    // sem1初始为0,阻塞在这里，等待sem_post
    sem_wait(&sem1);
    sum += num;
    sem_post(&sem2);
  }
  return NULL;
}