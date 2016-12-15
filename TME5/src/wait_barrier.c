#define _SVID_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>

int N;
int semid;
struct sembuf op;
void process (int NB_PCS) {
	printf ("avant barrière\n");
	wait_barrier (NB_PCS);
	printf ("après barrière\n");
	exit (0);
}

void P(int n){
	op.sem_op = -n;
	semop(semid,&op,1);
}

void V(int n){
	op.sem_op = n;
	semop(semid,&op,1);
}

void wait_barrier (int NB_PCS){
	V(1);
	P(NB_PCS);
	V(NB_PCS);
}


int main(int argc, char *argv[]){
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	key_t cle;
	cle = ftok(argv[0],getpid() & 11111111);
	semid = semget(cle,1,IPC_CREAT | IPC_EXCL | 0666);
	semctl(semid,0,SETVAL,0);
	op.sem_num = 0;
	op.sem_flg=0;
	N = atoi(argv[1]);
	int i;
	
	for(i=0 ; i<N;i++)
		if(fork()==0)
			process(N);
			
	
	for(i=0;i<N;i++)
		wait(NULL);
	
	semctl(semid, 0, IPC_RMID, 0);
	
	return EXIT_SUCCESS;
}

/* Avant barrière : init(sem, 0);
	
	barrière(int n){
		V(1);
		P(n); // teste si cpt sem >= n 
		V(n);
	}

	Avant barrière : init(sem, n);
	
	barrière(int n){
		P(1);
		Z(); // bloque tant que cpt != 0
	}

*/
