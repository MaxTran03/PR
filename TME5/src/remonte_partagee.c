#define _SVID_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>

int N;
int shm_id;
int *p_int, *adr_att;

void remonte_ipc(){
	int i, alea;
	printf("Début fonction\n");
	p_int = adr_att;
	
	for(i=0; i<N; i++){
		if(fork() == 0){
			srand(getpid());
			alea = (int) (10*(float)rand()/ RAND_MAX);
			printf("Le fils a choisi %d\n", alea);
			p_int[i] = alea;
			exit(0);
		}
	}
	
	for(i=0; i<N; i++)
		wait(NULL);
	
	int somme = 0;
	
	for(i=0; i<N; i++){
		somme += p_int[i];
	}
	
	printf("La somme des valeurs est: %d\n", somme);
	printf("Fin fonction\n");
}


int main(int argc, char *argv[]){
	key_t cle;
	struct shmid_ds *buf;
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	N = atoi(argv[1]);
	
	cle = ftok(argv[0], getpid() & 11111111);
	shm_id = shmget(cle, N, 0666 | IPC_CREAT);
	adr_att = shmat(shm_id, 0, 0600);
	
	remonte_ipc();
	
	shmdt(adr_att);
	shmctl(shm_id, IPC_RMID, buf);
	
	return EXIT_SUCCESS;
}




