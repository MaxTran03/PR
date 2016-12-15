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
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; /* la cond sur laquelle les thread attendent le main */
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER; /* la cond sur laquelle le main attend les thread*/

int cpt = 1;

pthread_t *tid;
int N;

void *func_thread(){
	
	if(cpt != N)
		pthread_create(&tid[cpt], NULL, func_thread, NULL);
	
	pthread_mutex_lock(&mutex);
	cpt++;
	pthread_mutex_unlock(&mutex);
	
	printf("je suis %d\n",pthread_self());
	pthread_mutex_lock(&mutex_cond2);
	pthread_cond_signal(&cond2);
	pthread_mutex_unlock(&mutex_cond2);
	pthread_cond_wait(&cond, &mutex_cond);
	pthread_mutex_unlock(&mutex_cond); /* IMPORTANT POUR DEBLOQUER LE PROCHAIN THREAD */
	printf("Self: %d\n", pthread_self());
	
	pthread_exit ((void*)0);
}

void f(){
	pthread_mutex_lock(&mutex_cond);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex_cond);
}

int main(int argc, char *argv[]){
	int i;
	sigset_t sig;	
	struct sigaction sigact;
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;	
	}
	
	N = atoi(argv[1]);
	tid = malloc(N*sizeof(pthread_t));
	
	sigfillset(&sig);
	sigdelset(&sig, SIGINT);
	sigact.sa_mask = sig;
	sigact.sa_flags = 0;
	sigact.sa_handler = f;
	sigaction (SIGINT, &sigact, NULL);
	sigprocmask(SIG_SETMASK, &sig, NULL);
	
	pthread_create(&tid[0], NULL, func_thread, NULL);
	
	for(i=0;i<N;i++){
		pthread_cond_wait(&cond2, &mutex_cond2);
		pthread_mutex_unlock(&mutex_cond2);
	}
	
	printf("Tous mes descendants sont créés\n");

	for(i=0; i<N; i++)
		pthread_join(tid[i],NULL);
		
	printf("Tous mes descendants se sont terminés\n");
	
	
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex_cond);
	pthread_mutex_destroy(&mutex_cond2);
	pthread_cond_destroy(&cond);
	pthread_cond_destroy(&cond2);
	
	return EXIT_SUCCESS;
}



