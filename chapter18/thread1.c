#include <stdio.h>
#include <pthread.h>


void* thread_routine(void* arg);

int main(int argc, char* argv[]){
	pthread_t tid;
	int thread_param = 5;
	if(pthread_create(&tid, NULL, thread_routine, (void*)&thread_param)==-1){
		printf("pthread_create() error\n");
		return -1;
	}
	sleep(10);
	puts("end of main");
	return 0;
}


void* thread_routine(void* arg){
	int cnt = *(int*)arg;
	for(int i = 0; i < cnt; ++i){
		sleep(1);
		puts("running thread\n");
	}
	return NULL;
}
