#define _SVID_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

struct sockaddr_in sini;
int sock, port, acc;
char rep[1024];

int main(int argc, char **argv){
	int fromlen;
	char c;
	int n, fd;
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	port = atoi(argv[1]);
	
	memset(&sini, '\0', sizeof(struct sockaddr_in));
	sini.sin_addr.s_addr = htonl(INADDR_ANY);
	sini.sin_family = AF_INET;
	sini.sin_port = htons(port);
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Erreur creation socket");
		exit(1);
	}
	
	if(bind(sock, (struct sockaddr *)&sini, sizeof(sini)) == -1){
		perror("Erreur au nommage");
		exit(1);
	}
	
	listen(sock, 1);
	fromlen = sizeof(sini);
	
	if((acc = accept(sock, (struct sockaddr *)&sini, &fromlen)) == -1){
		perror("accept");
		exit(1);
	}
		
	if(read(acc, &rep, sizeof(rep)) == -1){
		perror("recvfrom");
		exit(1);
	}
			
	strcat(rep, "2.txt");
	fd = open(rep, O_CREAT|O_TRUNC|O_RDWR, 0600);
	
	
	while((n = read(acc, &c, sizeof(char)))){
		write(fd, &c, sizeof(char));
	}
	
	close(acc);
	
	shutdown(sock, 2);
	close(sock);
	close(fd);
	
	return EXIT_SUCCESS;
}

/*

Socket en mode connecté (TCP):
------------------------------
Serveur:
1) Création du socket
2) Création de l'adresse
3) Etablir le bind 
4) Etablir le listen
5) accept (bloquant, se met en attente d'une connection. A l'arrivée d'une connexion, création d'une nouvelle socket. Cette fonction est liée à un fork, le fils gèrera la connexion et le père pourra alors faire un autre accept)
6) read / write (sur la nouvelle socket)
6-2) close(nouvelle socket)
7) Fermeture des sockets et des eventuels descripteurs

Client:
1) Création du socket
2) Récupération de l'adresse du serveur
3) Demande de connexion avec le serveur
4) write / read
4-bis) shutdown(w) (permet de fermer uniquement les read ou write)
5) Fermeture des sockets et des eventuels descripteurs

*/

