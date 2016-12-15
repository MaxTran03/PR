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
int sock, port, addr, acc;
char *rep;

struct message{
	int type;
	char content[1024];
}msg;


int main(int argc, char **argv){
	int i, fdu, n;
	
	char list[1024];
	char chaine[1024];
	char commande[1024];
	char fic[1024];
	char c[1024];
	
	FILE *file;
	
	if(argc != 3){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	addr = atoi(argv[1]);
	port = atoi(argv[2]);
	
	memset(&sini, '\0', sizeof(struct sockaddr_in));
	sini.sin_addr.s_addr = inet_addr(argv[1]);
	sini.sin_family = AF_INET;
	sini.sin_port = htons(port);
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Erreur creation socket");
		exit(1);
	}
	
	if(connect(sock, (struct sockaddr *) &sini, sizeof(sini)) == -1){
		perror("connect"); 
		exit(1);
	}
	
	strcpy(chaine, "");
	
	printf("Connected !\n");
	printf("Waiting command...\n\n");
	
	while(fgets(chaine, 1024, stdin)){
		strcpy(commande, "");
		strcpy(fic, "");
		strcpy(list, "");
		
		msg.type = 0;
		
		i = 0;
		while(chaine[i] != ' ' && chaine[i] != '\n'){
			strncat(commande, &chaine[i], 1);
			i++;
		}
		
		strcpy(fic, &chaine[i+1]);
		fic[strlen(fic)-1] = '\0';
		write(sock, chaine, 1024);
		
		printf("\nVérification de la commande:\n");
		printf("--> Commande: %s\n", commande);
		printf("--> Fichier: %s\n", fic);
		
		/* Lister les fichiers sur le serveur */
		if(strncmp(chaine, "LIST", 4) == 0){
			printf("\n##### DEBUT List #####\n");
			while(read(sock, list, 1024)){
				if(strncmp(list, "/fin", 4) == 0)
					break;
				printf("%s\n", list);
				strcpy(list, "");
			}
			printf("##### FIN List #####\n\n");
			continue;
		}
		
		/* Téléchargement de fichier depuis le client vers le serveur */
		if(strncmp(commande, "UPLOAD", 6) == 0){
			printf("\n##### DÉBUT Upload #####\n");
			printf("Fichier: %s\n", fic);
			file = fopen ( fic, "r" );
			
			while(fgets(c, 1024, file)){
				strcpy(msg.content,c);
				write(sock, &msg, sizeof(struct message));
			}
			
			msg.type = 1;
			write(sock, &msg, 1024);
			
			fclose(file);
			printf("Le fichier a bien été envoyé!\n");
			printf("##### FIN Upload #####\n\n");
		}
			
		/* Téléchargement de fichier depuis le serveur vers le client */
		if(strncmp(commande, "DOWNLOAD", 8) == 0){
			printf("\n##### DÉBUT Download #####\n");
			fdu = open(fic, O_CREAT|O_TRUNC|O_RDWR, 0600);
			
			while((n = read(sock, &msg, sizeof(struct message)))){
				if(msg.type == 1)
					break;
				write(fdu, msg.content, strlen(msg.content));
			}
			close(fdu);
			printf("Le fichier a bien été téléchargé!\n");
			printf("##### FIN Download #####\n\n");
		}
		
		strcpy(chaine, "");
	}
	
	shutdown(sock, 2);
	close(sock);

	return EXIT_SUCCESS;	
}

/*
Client FTP:
-----------

1) Socket
2) Récupération adresse serveur
3) Connect
4) Read / Write / Shutdown
5) Close

*/
