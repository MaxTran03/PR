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

struct message *spser, *spcl[MAX_USERS];
struct sigaction sigact;


struct id {
  int fd;
  char content[TAILLE_MESS];
};

struct id tab[MAX_USERS];
static jmp_buf env;

sem_t *s;
sem_t *s2[MAX_USERS];

void sigint_handler(){
	longjmp(env,1);
}


int main(int argc, char *argv[]){
	int shm_fd_ser, shm_fd_cl;
	if(argc != 2){
		perror("Error argument\n");
		return EXIT_FAILURE;
	}
	
	s = sem_open(argv[1],O_CREAT|O_RDWR,S_IRWXU,0);	
	
	sigact.sa_handler = sigint_handler;
   	sigaction(SIGINT, &sigact, NULL);
	
	if((shm_fd_ser = shm_open(argv[1], O_RDWR|O_CREAT|O_TRUNC, 0600)) == -1){
		perror("shm_open");
		exit(1);
	}

	if(ftruncate(shm_fd_ser, sizeof(struct message)) == -1){
		perror("ftruncate");
		exit(1);
	}

	if((spser = mmap(NULL, sizeof(struct message), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd_ser, 0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	
	int i;
	for(i=0; i<MAX_USERS; i++){
		strcpy(tab[i].content,"");
		tab[i].fd = -1;
	}

	while(1){
		if(setjmp(env) != 0)
			break;

		sem_wait(s);
		if(spser->type == 1){
			printf("Connexion...\n");
			if((shm_fd_cl = shm_open(spser->content, O_RDWR, 0600)) == -1){
				perror("shm_open");
				exit(1);
			}
			
			if(ftruncate(shm_fd_cl, sizeof(struct message)) == -1){
				perror("ftruncate");
				exit(1);
			}
			
			for(i=0; i<MAX_USERS; i++)
				if(strcmp(tab[i].content,"") == 0)
					break;
			
			if((spcl[i] = mmap(NULL, sizeof(struct message), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd_cl, 0)) == MAP_FAILED){
				perror("mmap");
				exit(1);
			}
			
			strcpy(tab[i].content, spser->content);
			tab[i].fd = shm_fd_cl;
			s2[i] = sem_open(spser->content, O_RDWR, S_IRWXU, 0);
			
			printf("Connected %s\n",spser->content);
	
		}else{
			if(spser->type == 3){
				
				for(i=0; i<MAX_USERS; i++)
					if(strcmp(tab[i].content, spser->content) == 0)
						break;
						
				strcpy(tab[i].content, "");
				tab[i].fd = -1;
				
				munmap(i, sizeof(struct message));
				shm_unlink(spser->content);
				
				sem_close(s2[i]);
				sem_unlink(spser->content);
				printf("Deconnexion %d\n",i);		
			}else{
				printf("Envoi du message -> %s\n", spser->content);
				
				char t[TAILLE_MESS];
				strcpy(t, spser->content);
				for(i=0; i<MAX_USERS; i++){
					/*printf("je boucle %d\n",i);*/
					
					if(strcmp(tab[i].content, "") == 0)
						continue;
					
					/*printf("jenvoie\n");*/
					
					strcpy(spcl[i]->content, spser->content);
					sem_post(s2[i]);
				}
			}
		}
	}
	
	sem_close(s);
	sem_unlink(argv[1]);
	
	munmap(spser, sizeof(struct message));
	
	shm_unlink(argv[1]);
	
	printf("\nLe programme s'est termine proprement\n");
	
	return EXIT_SUCCESS;
	
}
