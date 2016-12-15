#define _SVID_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

int N;
int sock;
struct sockaddr_un addr;
char *str;
int val;

void remonte_udp(){
	int i, alea;
	printf("Début fonction\n");
	
	for(i=0; i<N; i++){
		if(fork() == 0){
			srand(getpid());
			alea = (int) (10*(float)rand()/ RAND_MAX);
			printf("Le fils a choisi %d\n", alea);
			sendto(sock, &alea, sizeof(int), 0, (struct sockaddr *)&addr, sizeof(addr));
			exit(0);
		}
	}
	
	for(i=0; i<N; i++)
		wait(NULL);
	
	int somme = 0;
	
	for(i=0; i<N; i++){
		recvfrom(sock, &val, sizeof(int), 0, 0, sizeof(addr));
		somme += val;
	}
	
	printf("La somme des valeurs est: %d\n", somme);
	printf("Fin fonction\n");
}


int main(int argc, char *argv[]){
	
	if(argc != 3){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	N = atoi(argv[2]);
	
	memset(&addr, '\0', sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "./mysock");
	
	if((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1){
		perror("Erreur creation socket");
		exit(1);
	}
	
	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		perror("Erreur au nommage");
		exit(1);
	}
	
	remonte_udp();
	
	unlink("./mysock");
	
	close(sock);
	
	return EXIT_SUCCESS;
}

/*

Socket en mode non connecté (AF_UNIX ou UDP):
---------------------------------------------
Serveur:
1) Création du socket
2) Création de l'adresse
3) Etablir le bind
4) recvfrom (lire la requête)/ sendto (répondre)
5) Fermeture de la socket

Client:
1) Création du socket
2) Récupération de l'adresse du serveur
3) sendto (envoyer une requête) / recfrom (lire la réponse)
4) Fermeture de la socket

*/



