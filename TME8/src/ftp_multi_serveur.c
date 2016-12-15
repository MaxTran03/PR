#define _SVID_SOURCE 1
#define _XOPEN_SOURCE 700 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

struct sockaddr_in sin;
int sock, port, addr, acc;
char rep[1024];

struct message{
	int type;
	char content[1024];
};

struct message msg;

int main(int argc, char **argv){
	if(argc != 3){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	port = atoi(argv[1]);
	
	memset(&sin, '\0', sizeof(struct sockaddr_in));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Erreur creation socket");
		exit(1);
	}
	
	if(bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1){
		perror("Erreur au nommage");
		exit(1);
	}
	
	listen(sock, 1);
	int fromlen = sizeof(sin);
	
	mkdir(argv[2], O_CREAT|O_TRUNC|0600);
	
	int i;
	int fdu;
	
	while(1){
		
		if((acc = accept(sock, (struct sockaddr *)&sin, &fromlen)) == -1){
			perror("accept");
			exit(1);
		}
		
		if(fork() == 0){
			char saisie[1024];
			char commande[1024];
			char fic[1024];
			char path[1024];
			strcpy(path, "");
			strcpy(saisie, "");
		
			msg.type = 0;
		
			printf("Connected !\n");
			printf("Waiting request from Client...\n\n");
		
			while(read(acc, saisie, 1024)){
				printf("%s\n",saisie);
			
				/* Réinitialisation des tableaux */
				strcpy(commande, "");
				strcpy(fic, "");
				strcpy(path, "");
			
				i = 0;
				msg.type = 0;
			
				DIR *pt_Dir;
				struct dirent* dirEnt;
			
				/* Lister les fichiers sur le serveur */
				if(strcmp(saisie, "LIST\n") == 0){
					printf("\n##### DEBUT List #####\n");
					pt_Dir = opendir(argv[2]); 
					while((dirEnt = readdir(pt_Dir)) != NULL) {
						/*printf ("%s\n", dirEnt->d_name);*/
						write(acc, dirEnt->d_name, 1024);
					}
					write(acc, "/fin", strlen("/fin"));
					printf("Liste du contenu du serveur envoyé au client!\n");
					printf("##### FIN List #####\n\n");
					closedir(pt_Dir);
					continue;
				}
			
				while(saisie[i] != ' ' && saisie[i] != '\n'){
					strncat(commande, &saisie[i], 1);
					i++;
				}

				strcpy(fic, &saisie[i+1]);
				fic[strlen(fic)-1] = '\0';
			
				printf("\nVérification de la commande:\n");
				printf("--> Commande: %s\n", commande);
				printf("--> Fichier: %s\n", fic);
			
				/* Téléchargement de fichier depuis le serveur vers le client */
				if(strncmp(commande, "DOWNLOAD", 8) == 0){
					printf("\n##### DÉBUT Download #####\n");
					strcat(path, argv[2]);
					strcat(path, "/");
					strcat(path, fic);
					FILE *file = fopen ( path, "r" );
					char c[1024];
	
					while(fgets(c, 1024, file)){
						strcpy(msg.content,c);
						write(acc, &msg, sizeof(struct message));
					}
				
					msg.type = 1;
					write(acc, &msg, 1024);
					printf("Le download a été effectué correctement!\n");
					printf("##### FIN Download #####\n\n");
				}
			
				/* Téléchargement de fichier depuis le client vers le serveur */
				if(strncmp(commande, "UPLOAD", 6) == 0){
					printf("\n##### DÉBUT Upload #####\n");
					strcat(path, argv[2]);
					strcat(path, "/");
					strcat(path, fic);
					printf("Chemin: %s\n", path);
					fdu = open(path, O_CREAT|O_TRUNC|O_RDWR, 0600);
					char c;
					int n;
				
					while(n = read(acc, &msg, sizeof(struct message))){
						if(msg.type == 1)
							break;
						write(fdu, msg.content, strlen(msg.content));
					}
					close(fdu);
					printf("Le upload a été effectué correctement!\n");
					printf("##### FIN Upload #####\n\n");
				}
				strcpy(saisie, "");
			}
			close(acc);
			exit(1);
		}
	}
	
	shutdown(sock, 2);
	close(sock);
	return EXIT_SUCCESS;
}
