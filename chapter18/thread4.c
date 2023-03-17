#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 100

void *thread_inc(void *arg);
void *thread_des(void *arg);

long long num = 0;

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; ++i) {
    if (i % 2) {
      pthread_create(&threads[i], NULL, thread_inc, NULL);
    } else {
      pthread_create(&threads[i], NULL, thread_des, NULL);
    }
  }
  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_join(threads[i], NULL);
  }
  printf("result %lld\n", num);
  return 0;
}

void *thread_inc(void *arg) {
  for (int i = 0; i < 5000; ++i) {
    num++;
  }
  return NULL;
}
void *thread_des(void *arg) {
  for (int i = 0; i < 5000; ++i) {
    num--;
  }
  return NULL;
}