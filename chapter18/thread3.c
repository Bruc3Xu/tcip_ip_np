#include <pthread.h>
#include <stdio.h>

int sum = 0;
void *summation(void *arg);

int main(int argc, char *argv[])

{
  pthread_t tid1, tid2;
  int range1[] = {1, 5};
  int range2[] = {6, 10};
  pthread_create(&tid1, NULL, summation, (void *)range1);
  pthread_create(&tid2, NULL, summation, (void *)range2);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  printf("result is %d\n", sum);
  return 0;
}

void *summation(void *arg) {
  int start = ((int *)arg)[0];
  int end = ((int *)arg)[1];
  for (int i = start; i <= end; ++i) {
    // 这里直接修改sum是不安全的
    sum += i;
  }
  return NULL;
}
