#define _XOPEN_SOURCE 700
#define _REENTRANT

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cond = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cond2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

int max = 5;
int cpt= 0;
void* thread_func(void* arg) {
	cpt++;
	printf("je suis %d\n",pthread_self());
	int i, nb;
	int *param;
	int *lvl = (int*)arg;
	pthread_t *tid;
	
	nb = (*lvl)+1;
	
	if(*lvl < max){
		param = (int*)malloc(sizeof(int));
		*param = nb;
		tid = calloc(nb, sizeof(pthread_t));
		printf("%d cree %d fils\n", (int)pthread_self(), nb);
		
		for(i=0; i<nb; i++){
			pthread_create((tid+i), 0, thread_func, param);
		}
		pthread_mutex_lock(&mutex_cond2);
		pthread_cond_signal(&cond2);
		pthread_mutex_unlock(&mutex_cond2);
		
		for (i = 0; i < nb; i++)
      	pthread_join(tid[i], NULL);
  	}

	if (*lvl > 1)
		pthread_exit((void*)0);

	
	return (void*)0;
}

int main(int argc, char *argv[]){
	pthread_t tid2;
	
	pthread_create(&tid2, NULL, thread_func, argv);
	pthread_cond_wait(&cond2, &mutex_cond2);
	
	pthread_join(tid2, NULL);
	
	printf("Il y a %d de thread créés\n",cpt);
	
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex_cond);
	pthread_mutex_destroy(&mutex_cond2);
	pthread_cond_destroy(&cond);
	pthread_cond_destroy(&cond2);
	
	return 0;
}

/*  */
