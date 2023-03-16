#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void* thread_routine(void* arg);

int main(int argc, char* argv[]){
	pthread_t tid;
	int thread_param = 5;
	if(pthread_create(&tid, NULL, thread_routine, (void*)&thread_param)==-1){
		printf("pthread_create() error\n");
		return -1;
	}
	void* thr_ret;
	if(pthread_join(tid, &thr_ret)==-1){
		printf("pthread_join() error\n");
		return -1;
	}
	printf("thread finished, msg %s\n", (char*)thr_ret);
	free(thr_ret);
	return 0;
}


void* thread_routine(void* arg){
	int cnt = *(int*)arg;
	for(int i = 0; i < cnt; ++i){
		sleep(1);
		puts("running thread\n");
	}
	char* msg =  malloc(sizeof(char)*50);
	strcpy(msg, "I'm thread.\n");
	return (void*)msg;
}
