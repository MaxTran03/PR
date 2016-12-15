#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include <setjmp.h>
#include <errno.h>
#include "../include/chat_common.h"

struct message *spcl, *spser;
struct sigaction sigact;

static jmp_buf env;
char N[TAILLE_MESS];

sem_t *s;
sem_t *s2;

void sigint_handler(){
	spser->type = 3;
	strcpy(spser->content, N);
	sem_post(s);
	longjmp(env,1);
}


void *func_thread_read(){
	while(1){
		sem_wait(s2);	
		printf("%s\n", spcl->content);
	}
	
	pthread_exit((void*)0);
	return NULL;
}

void *func_thread_write(){
	char c;
	char t[TAILLE_MESS];
	spser->type = 1;
	strcpy(spser->content, N);
	sem_post(s);
	
	while(1){
		strcpy(t,"");
		while((c = getchar()) != '\n'){
			strcat(t, &c);
		}
		spser->type = 2;
		strcpy(spser->content, t);
		printf("Envoi de message -> %s\n", spser->content);
		sem_post(s);
	}
	
	pthread_exit((void*)0);
	return NULL;
}


int main(int argc, char *argv[]){
	int shm_fd_cl, shm_fd_ser;
	int tid, tid2;
	sigset_t sig;
	
	if(argc != 3){
		perror("Error argument\n");
		return EXIT_FAILURE;
	}
	
	sigfillset(&sig);
	sigdelset(&sig,SIGINT);
	
	s = sem_open(argv[2],O_RDWR,S_IRWXU,0);
	s2 = sem_open(argv[1],O_CREAT|O_RDWR,S_IRWXU,0);
	
	sigact.sa_handler = sigint_handler;
   sigaction(SIGINT, &sigact, NULL);

   strcpy(N, argv[1]);
 
	if((shm_fd_cl = shm_open(argv[1], O_RDWR|O_CREAT|O_TRUNC, 0600)) == -1){
		perror("shm_open_cl");
		exit(1);
	}
	
	if((shm_fd_ser = shm_open(argv[2], O_RDWR, 0600)) == -1){
		perror("shm_open_ser");
		exit(1);
	}
	
	if(ftruncate(shm_fd_cl, sizeof(struct message)) == -1){
		perror("ftruncate");
		exit(1);
	}

	if(ftruncate(shm_fd_ser, sizeof(struct message)) == -1){
		perror("ftruncate");
		exit(1);
	}
	
	if((spcl = mmap(NULL, sizeof(struct message), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd_cl, 0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}

	if((spser = mmap(NULL, sizeof(struct message), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd_ser, 0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}

	pthread_create(&tid, NULL, func_thread_read, NULL);
	pthread_create(&tid2, NULL, func_thread_write, NULL);
	
	if(setjmp(env) == 0){
		sigsuspend(&sig);
	}
	
	munmap(spcl, sizeof(struct message));
	
	
	sem_close(s2);
	sem_unlink(argv[1]);
	shm_unlink(argv[1]);
	
	printf("\nLe programme s'est termine proprement\n");	
	
	return EXIT_SUCCESS;
	
}
