#define _XOPEN_SOURCE 700
#define _REENTRANT

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

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
		for (i = 0; i < nb; i++)
      	pthread_join(tid[i], NULL);
  	}

	if (*lvl > 1)
		pthread_exit((void*)0);

	
	return (void*)0;
}

int main(){
	int tab = 3;
	pthread_t tid2;
	pthread_create(&tid2, NULL, thread_func, (void*)&tab);
	pthread_join(tid2, NULL);
	
	printf("%d\n",cpt);
	
	return 0;
}

/* 
10 threads créés en comptant le main
Soit Un le nombre de threads au rang n: Un+1 = (n+1)Un -> Un = n! */
