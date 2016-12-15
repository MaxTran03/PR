#define _XOPEN_SOURCE 700
#define _REENTRANT

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>

#define N 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condP = PTHREAD_COND_INITIALIZER;
pthread_cond_t condC = PTHREAD_COND_INITIALIZER;

int stack_size = 0;
char stack[N]={'\0'};

void push(char c){
	pthread_mutex_lock(&mutex);
	
	while(stack_size == N)
		pthread_cond_wait(&condP, &mutex);
	
	stack[stack_size] = c;
	
	printf ("PUSH : %c %d\n", c, pthread_self());
	
	stack_size++;
	
	if(stack_size == 1)
		pthread_cond_broadcast(&condC);	

	pthread_mutex_unlock(&mutex);
}

char pop(){
	pthread_mutex_lock(&mutex);
	char res = '\0';
	while(stack_size == 0)
		pthread_cond_wait(&condC, &mutex);
	
	res = stack[stack_size-1];
	
	printf ("POP : %c %d\n", res, pthread_self());
	
	stack_size--;
	stack[stack_size] = '\0';
	
	if(stack_size == (N-1))
		pthread_cond_broadcast(&condP);

	pthread_mutex_unlock(&mutex);

	return res;
}

void *Producteur(){
	int c;
	while((c = getchar()) != EOF){
		push(c);
	}
	
	pthread_exit ((void*)0);
}

void *Consommateur(){
	while(1){
		putchar(pop());
		fflush(stdout);
	}
	
	pthread_exit ((void*)0);	
}


int main(int argc, char *argv[]){
	if(argc != 3){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	int NBPROD = atoi(argv[1]);
	int NBCONS = atoi(argv[2]);
	
	int i;
	pthread_t tidP[NBPROD];
	for(i=0; i<NBPROD; i++)
		pthread_create(&tidP[i], NULL, Producteur, NULL);
	
	pthread_t tidC[NBCONS];
	for(i=0; i<NBCONS; i++)
		pthread_create(&tidC[i], NULL, Consommateur, NULL);

	for(i=0; i<NBPROD; i++)
		pthread_join(tidP[i],NULL);
		
	for(i=0; i<NBCONS; i++)
		pthread_join(tidC[i],NULL);
		
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&condP);
	pthread_cond_destroy(&condC);
	
	return EXIT_SUCCESS;
}
