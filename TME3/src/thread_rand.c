#define _POSIX_SOURCE 700

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

int somme;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* affiche(){

	int alea = (int) (10*((double)rand())/ RAND_MAX);
	
	pthread_mutex_lock(&mutex);
	somme += alea;
	pthread_mutex_unlock(&mutex);
	
	printf("Numero de aleatoire : %d, numero d'identité : %d\n", alea, (int)pthread_self());
	pthread_exit((void*)somme);
	return NULL;
}


int main(){
	int status;
	pthread_t tid[N];
	int i,j;
	int* pt_ind;
	
	somme = 0;
	
	for(i=0; i<N; i++){
		pthread_create(&tid[i], NULL, affiche, NULL); 
	}
	
	for(j=0; j<N; j++){
		pthread_join(tid[j], (void**)&status);
	}
	
	printf("Valeur final : %d\n", (int)status);
	
	return 0;

}
