#define _SVID_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>

int N;
int msg_id;

struct message{
	long type;
	int val;
}msg;


void remonte_ipc(){
	int i;
	msg.type = 1;
	printf("Début fonction\n");
	for(i=0; i<N; i++){
		if(fork() == 0){
			srand(getpid());
			int alea = (int) (10*(float)rand()/ RAND_MAX);
			printf("Le fils a choisi %d\n", alea);
			msg.val = alea;
			msgsnd(msg_id, &msg, sizeof(int), 0);
			exit(0);
		}
	}
	
	for(i=0; i<N; i++)
		wait(NULL);
	
	int somme = 0;
	for(i=0; i<N; i++){
		msgrcv(msg_id, &msg, sizeof(int), 1, 0);
		somme += msg.val;
	}
	
	printf("La somme des valeurs est: %d\n", somme);
	printf("Fin fonction\n");
}


int main(int argc, char *argv[]){
	key_t cle;
	struct msqid_ds *buf;
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	N = atoi(argv[1]);
	
	cle = ftok(argv[0], getpid() & 11111111);
	msg_id = msgget(cle, 0666 | IPC_CREAT);
	
	remonte_ipc();
	
	msgctl(msg_id, IPC_RMID, &buf);
	
	return EXIT_SUCCESS;
}




