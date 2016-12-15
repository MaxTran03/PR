#define _POSIX_SOURCE 700

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10
/*
int g;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int cpt=0;
pthread_t tid[N];

void* affiche(void* arg){
	if(((int*)arg) == 0){
		int i;
		pthread_cond_wait(&cond,&mutex);
		printf("Valeur final : %d\n", g);
		return NULL;
	}
	
	
	int alea = (int) (10*((double)rand())/ RAND_MAX);
	
	srand(pthread_self());
	printf("%d\n",alea);
	
	pthread_mutex_lock(&mutex);
	g += alea;
	cpt++;
	pthread_mutex_unlock(&mutex);
	
	
	if(((int*)arg)==(N-1)){
		while(cpt!=N-1){}
		pthread_cond_signal(&cond);
	}
	return NULL;
}


int main(){
	int status;

	int i,j;
	int* pt_ind;
	
	g = 0;
	
	for(i=0; i<N; i++){
		pt_ind = (int*)malloc(sizeof(i));
		*pt_ind = i;
		pthread_create(&tid[i], NULL, affiche, (void*)*pt_ind); 
	}
	
	for(j=0; j<N; j++){
		pthread_join(tid[j], (void**)&status);
	}
	
	return 0;

}*/

int somme;
pthread_mutex_t mutex_somme = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cond = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int cpt = 0;
pthread_t tid[N];
pthread_t tid2;

void* affiche(){
	pthread_cond_wait(&cond,&mutex_cond);
	pthread_mutex_unlock(&mutex_cond);
	printf("Valeur final : %d\n", somme);
	
	return NULL;
}

void* func_thread(){
	
	int alea = (int) (10*((double)rand())/ RAND_MAX);
	
	srand(pthread_self());
	printf("%d\n",alea);

	pthread_mutex_lock(&mutex_somme);
	somme += alea;
	cpt++;
	pthread_mutex_unlock(&mutex_somme);
	
	if(cpt == N){
		pthread_mutex_lock(&mutex_cond);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex_cond);
	}
	
	pthread_exit((void*)0);
	
	return NULL;
}


int main(){
	int status;

	int i;
	int* pt_ind;
	
	pthread_mutex_lock(&mutex_cond);
	
	for(i=0; i<N; i++){
		pt_ind = (int*)malloc(sizeof(i));
		*pt_ind = i;
		pthread_create(&tid[i], NULL, func_thread, (void*)*pt_ind);
	}
	
	pthread_create(&tid2, NULL, affiche, (void*)*pt_ind); 
	
	for(i=0; i<N; i++)
		pthread_join(tid[i], (void**)&status);
		
	pthread_join(tid2, (void**)&status);
	
	free(pt_ind);
	pthread_mutex_destroy(&mutex_somme);
	pthread_mutex_destroy(&mutex_cond);
	pthread_cond_destroy(&cond);
	
	return EXIT_SUCCESS;
}
