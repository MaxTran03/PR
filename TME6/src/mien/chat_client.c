#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include "../include/chat_common.h"

struct sigaction sigact;
struct myshm *sp_client, *sp_serveur;
char *client_shm, *serveur_shm;
sem_t *s;

void *func_thread_read(){
	printf("Début READ\n");
	
	while(1){
		sem_wait(&sp_client->sem);
		
		printf("%s\n", sp_client->messages[sp_client->read].content);
	}
	
	printf("Fin READ\n");
}

void *func_thread_write(){
	char str[TAILLE_MESS];
	char mess[TAILLE_MESS];
	
	printf("Début WRITE\n");
	
	while(1){
		printf("Début Entrée Standart\n");
		
		fgets(str, TAILLE_MESS, stdin);
		
		strcpy(mess, client_shm);
		strcat(mess, ":");
		strcat(mess, str);
		
		sem_wait(&sp_serveur->sem);
		
		strcpy(sp_serveur->messages[sp_serveur->write].content, mess);
		sp_serveur->write++;
		sp_serveur->write %= MAX_MESS;
		
		sem_post(&sp_serveur->sem);
	}

	printf("FIN WRITE\n");
}

void connexion(){
	printf("Connexion...\n");
	sem_wait(&sp_serveur->sem);
	
	sp_serveur->messages[sp_serveur->write].type = 1;
	strcpy(sp_serveur->messages[sp_serveur->write].content, client_shm);
	sp_serveur->write++;
	sp_serveur->write %= MAX_MESS;
		
	sem_post(&sp_serveur->sem);
	printf("Connected!\n");
}

void deconnexion(){
	printf("Deconnexion...\n");
	sem_wait(&sp_serveur->sem);
	
	sp_serveur->messages[sp_serveur->write].type = 3;
	strcpy(sp_serveur->messages[sp_serveur->write].content, client_shm);
	sp_serveur->write++;
	sp_serveur->write %= MAX_MESS;
		
	sem_post(&sp_serveur->sem);
	
	/* Détachement du segment */
	munmap(sp_serveur, sizeof(int));
	munmap(sp_client, sizeof(int));
	
	/* Destruction du segment */
	shm_unlink("shm_emission");
	shm_unlink("shm_reception");
	
	/* Destruction du semaphore */
	sem_destroy(&sp_serveur->sem);
	sem_destroy(&sp_client->sem);
	
	printf("Deconnected!\n");
}


int main(int argc, char *argv[]){
	int shm_fd_serveur, shm_fd_client;
	pthread_t reception, emission;
	
	if(argc != 3){
		perror("Error argument\n");
		return EXIT_FAILURE;
	}
	
	client_shm = argv[1];
	serveur_shm = argv[2];
	
	sigset_t sig;
	
	sigfillset(&sig);
	sigdelset(&sig, SIGINT);
	sigprocmask(SIG_SETMASK, &sig, NULL);
	
	sigact.sa_handler = deconnexion;
	sigaction(SIGINT, &sigact, 0);
	
	/*if((s = sem_open("monsem", O_CREAT|O_RDWR, 0666, 1)) == SEM_FAILED)
		printf("CACA\n");
	sp_client->sem = *s;*/
	
	printf("Début Client...\n");
	
	/* Création d'un segment de mémoire partagée pour l'émission */
	if((shm_fd_serveur = shm_open("shm_emission", O_RDWR|O_CREAT, 0600)) == -1){
		perror("shm_open");
		exit(1);
	}
	/* Création d'un segment de mémoire partagée pour la réception */
	if((shm_fd_client = shm_open("shm_reception", O_RDWR|O_CREAT, 0600)) == -1){
		perror("shm_open");
		exit(1);
	}
	
	
	/* Allouer au segment d'émission une taille */
	if(ftruncate(shm_fd_serveur, sizeof(int)) == -1){
		perror("ftruncate");
		exit(1);
	}
	/* Allouer au segment de réception une taille */
	if(ftruncate(shm_fd_client, sizeof(int)) == -1){
		perror("ftruncate");
		exit(1);
	}
	
	
	/* Mapper le segment */
	if((sp_serveur = mmap(NULL, sizeof(struct myshm), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd_serveur, 0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	/* Mapper le segment */
	if((sp_client = mmap(NULL, sizeof(struct myshm), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd_client, 0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	


	pthread_create(&reception, NULL, func_thread_read, NULL);
	pthread_create(&emission, NULL, func_thread_write, NULL);
	
	pthread_join(reception, NULL);
	printf("Après join\n");
	/*
	pthread_join(emission, NULL);*/
	
	connexion();
	
	return EXIT_SUCCESS;
	
}
