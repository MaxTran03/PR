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
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int stack_size = 0;
char stack[N]={'\0'};

void push(char c){
	pthread_mutex_lock(&mutex);
	
	while(stack_size == N)
		pthread_cond_wait(&cond, &mutex);
	
	stack[stack_size] = c;
	
	printf ("PUSH : %c\n", c);
	
	stack_size++;
	
	if(stack_size == 1)
		pthread_cond_signal(&cond);	

	pthread_mutex_unlock(&mutex);
}

char pop(){
	pthread_mutex_lock(&mutex);
	char res = '\0';
	while(stack_size == 0)
		pthread_cond_wait(&cond, &mutex);
	
	res = stack[stack_size-1];
	
	printf ("POP : %c\n", res);
	
	stack_size--;
	stack[stack_size] = '\0';
	
	if(stack_size == (N-1))
		pthread_cond_signal(&cond);

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


int main(){
	pthread_t tid[2];
	pthread_create(&tid[0], NULL, Producteur, NULL);
	pthread_create(&tid[1], NULL, Consommateur, NULL);
	
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	
	return 0;
}
