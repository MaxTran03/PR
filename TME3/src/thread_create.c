#define _POSIX_SOURCE 700

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

void* affiche(void* arg){

	printf("Numero de creation : %d, numero d'identité : %d\n", (int)arg, (int)pthread_self());
	pthread_exit((void*)(2*(int)arg));
	
	return NULL;
}


int main(){
	int status;
	pthread_t tid[N];
	int i,j;
	int* pt_ind;
	
	for(i=0; i<N; i++){
		pt_ind = (int*)malloc(sizeof(i));
		*pt_ind = i;
		pthread_create(&tid[i], NULL, affiche, (void*)*pt_ind); 
	}
	
	for(j=0; j<N; j++){
		pthread_join(tid[j], (void**)&status);
		printf("Numero d'identité : %d, Numero de terminaison %d\n",(int)tid[j], (int)status);
	}
	
	free(pt_ind);
	return 0;

}
