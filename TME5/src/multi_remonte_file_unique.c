#define _SVID_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>

int N;
int msg_id;

key_t cle;
struct msqid_ds *buf;

struct message{
	long type;
	int val;
	int c;
}msgP,msgF;

void remonte_ipc(){
	int i, j;
	
	
	
	
	printf("Début fonction\n");
	
	for(i=0; i<N; i++){
		if(fork() == 0){
			msgF.type = 1;
			msgF.c = i+2;
			int somme = 0;
			
			srand(getpid());
			int max_msg = (int) (N*(float)rand()/ RAND_MAX);

			printf("Le fils %d veut recevoir %d message du père\n", i+2 ,max_msg);
			msgF.val = max_msg;
			msgsnd(msg_id, &msgF, sizeof(struct message), 0);
			
			for(j=0; j<max_msg; j++){
				msgrcv(msg_id , &msgP, sizeof(struct message), i+2, 0);
				somme += msgP.val;
			}
			printf("La somme des valeurs est: %d pour le fils %d\n", somme, i+2);
			exit(0);
		}
	}
	
	
	for(i=0; i<N; i++){
		msgrcv(msg_id, &msgF, sizeof(struct message), 1, 0);
		msgP.type = msgF.c;
		printf("Le pere recoit %d du fils %d\n", msgF.val,msgF.c);
		for(j=0;j<msgF.val;j++){
			int r = (int) (100*(float)rand()/ RAND_MAX);
			msgP.val = r;
			printf("--> Le père fait rand %d pour le fils %d\n", msgP.val, msgF.c);
			msgsnd(msg_id, &msgP, sizeof(struct message), 0);
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
	
	N = atoi(argv[1]);
	cle = ftok(argv[0], getpid() & 11111111);
	msg_id = msgget(cle, 0666 | IPC_CREAT);
	
	remonte_ipc();
	
	msgctl(msg_id, IPC_RMID, &buf);
	
	return EXIT_SUCCESS;
}


/* Utiliser les types dans la file
	- 1 identifiant pour que chaque fils informe son père du nb de valeurs souhaité
	- 1 identifiant pour chaque fils à qui le père envoie des valeurs.
	-> 2n identifiants
 */
