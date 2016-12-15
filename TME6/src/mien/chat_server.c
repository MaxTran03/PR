#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <setjmp.h>
#include "../include/chat_common.h"

struct sigaction sigact;
struct myshm *sp_serv;
char *client_shm, *serveur_shm;
sem_t s;

static jmp_buf env;

void sigint_handler(){
	longjmp(env, 1);
}

int main(int argc, char *argv[]){
	int shm_fd_serv;
	
	if(argc != 2){
		perror("Error argument\n");
		return EXIT_FAILURE;
	}
	
	client_shm = argv[1];
	serveur_shm = argv[2];
	
	sigact.sa_handler = sigint_handler;
	sigaction(SIGINT, &sigact, 0);
	
	printf("Début Serveur...\n");
	
	/* Création d'un segment de mémoire partagée */
	if((shm_fd_serv = shm_open("shm_serv", O_RDWR|O_CREAT, 0600)) == -1){
		perror("shm_open");
		exit(1);
	}
	
	/* Allouer au segment une taille */
	if(ftruncate(shm_fd_serv, sizeof(int)) == -1){
		perror("ftruncate");
		exit(1);
	}
	
	/* Mapper le segment */
	if((sp_serv = mmap(NULL, sizeof(struct myshm), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd_serv, 0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	
	while(1){
		
		if(sp_serv->messages[sp_serv->write].type == 1){
			/* Connexion */
			printf("Etablissement de la connexion!\n");
			
		}else if(sp_serv->messages[sp_serv->write].type == 3){
			/* Deconnexion */
		}else{
			/* Diffusion */
			int i;
			for(i=0; i<MAX_USERS; i++){
				
			}
		}
	}
	
	/* Détachement du segment */
	munmap(sp_serv, sizeof(int));
	
	/* Destruction du segment */
	shm_unlink("shm_serv");
	
	/* Destruction du semaphore */
	sem_destroy(&sp_serv->sem);
	
	printf("\nFin du programme\n");
	
	return EXIT_SUCCESS;
}
