#define _SVID_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>

int N;
char *M;
int msg_id;
int *id_fils;

key_t *cle;
struct msqid_ds *buf;

struct message{
	long type;
	int val;
	int id;
}msgP,msgF;

void remonte_ipc(){
	int i, j;
	
	msgP.type = 1;
	msgF.type = 1;
	
	printf("Début fonction\n");
	
	for(i=0; i<N; i++){
		if(fork() == 0){
			int somme = 0;
			cle[i] = ftok(M, getpid() & 11111111);
			id_fils[i] = msgget(cle[i+1], 0666 | IPC_CREAT);
			srand(getpid());
			int max_msg = (int) (N*(float)rand()/ RAND_MAX);

			printf("Le fils %d veut recevoir %d message du père\n", id_fils[i],max_msg);
			msgF.val = max_msg;
			msgF.id = id_fils[i];
			msgsnd(msg_id, &msgF, sizeof(struct message), 0);
			
			for(j=0; j<max_msg; j++){
				msgrcv(msgF.id, &msgP, sizeof(struct message), 1, 0);
				somme += msgP.val;
			}
			printf("La somme des valeurs est: %d pour le fils %d\n", somme, msgF.id);
			exit(0);
		}
	}
	
	
	
	for(i=0; i<N; i++){
		msgrcv(msg_id, &msgF, sizeof(struct message), 1, 0);
		for(j=0;j<msgF.val;j++){
			int r = (int) (100*(float)rand()/ RAND_MAX);
			msgP.val = r;
			printf("Le père fait un rand %d pour le fils %d\n", msgP.val, msgF.id);
			msgsnd(msgF.id, &msgP, sizeof(struct message), 0);
		}	
	}
	
	for(i=0; i<N; i++)
		wait(NULL);
		
	printf("Fin fonction\n");
}


int main(int argc, char *argv[]){
	
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	M = argv[0];
	N = atoi(argv[1]);
	id_fils = (int*)malloc(N*sizeof(int));
	cle = (key_t*)malloc((N+1)* sizeof(key_t));
	cle[0] = ftok(argv[0], getpid() & 11111111);
	msg_id = msgget(cle[0], 0666 | IPC_CREAT);
	
	remonte_ipc();
	
	msgctl(msg_id, IPC_RMID, &buf);
	
	free(id_fils);
	free(cle);
	
	return EXIT_SUCCESS;
}




