#define _POSIX_SOURCE 700

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NB_THREADS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cond = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int cpt = 1;
pthread_t tid[NB_THREADS];

void wait_barrier(int n){
		
	if(cpt != n){
		pthread_mutex_lock(&mutex);
		cpt++;
		pthread_mutex_unlock(&mutex);
		
		pthread_cond_wait(&cond, &mutex_cond);
		pthread_mutex_unlock(&mutex_cond);
	}else{
		pthread_mutex_lock(&mutex_cond);
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex_cond);
	}
}



void* thread_func (void *arg) {
   printf("avant barriere\n");
   wait_barrier (NB_THREADS);
   printf("après barriere\n");
   pthread_exit((void*)0);
}


int main(){
	int status;

	int i, j;
	int* pt_ind;
	
	pthread_mutex_lock(&mutex_cond);
	
	for(i=0; i<NB_THREADS; i++){
		pt_ind = (int*)malloc(sizeof(i));
		*pt_ind = i;
		pthread_create(&tid[i], NULL, thread_func, (void*)*pt_ind);
	}
	
	for(j=0; j<NB_THREADS; j++)
		pthread_join(tid[j], (void**)&status);
	
	free(pt_ind);
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex_cond);
	pthread_cond_destroy(&cond);
	
	return 0;

}
