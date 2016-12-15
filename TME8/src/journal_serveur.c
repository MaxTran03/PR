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
#include <sys/select.h>


int main(int argc, char** argv){
	int N = argc-1;
	struct sockaddr_in sin[N];
	int sock[N], port[N], acc[N];
	int i, j, fromlen, fd;
	fd_set mselect;
	char host[64], file;
	
	if(argc < 2){
		perror("Erreur argument!\n");
		return EXIT_FAILURE;
	}
	
	printf("Début MAIN\n");
	
	for(i=0; i<N; i++){
		port[i] = atoi(argv[i+1]);
		memset(&sin[i], '\0', sizeof(struct sockaddr_in));
		sin[i].sin_addr.s_addr = htonl(INADDR_ANY);
		sin[i].sin_family = AF_INET;
		sin[i].sin_port = htons(port[i]);
	
		if((sock[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1){
			perror("Erreur creation socket");
			exit(1);
		}

		if(bind(sock[i], (struct sockaddr *)&sin[i], sizeof(sin[i])) == -1){
			perror("Erreur au nommage");
			exit(1);
		}
	
		listen(sock[i], 1);
		fromlen = sizeof(sin[i]);
		
	}
	
	fd = open("cx.log", O_CREAT|O_TRUNC|O_RDWR, 0600);
	
	i = 0;
	while(i<N){
		printf("Client %d\n", i);
		
		FD_ZERO(&mselect);
		for(j=0; j<N; j++){
			FD_SET(sock[j], &mselect);
		}
		
		if(select(sock[N-1]+1, &mselect, NULL, NULL, NULL) == -1){
			perror("select");
			exit(3);
		}
		
		for(j=0; j<N; j++){
			if(FD_ISSET(sock[j], &mselect)){
				if((acc[j] = accept(sock[j], (struct sockaddr *)&sin[j], &fromlen)) == -1){
					perror("accept");
					exit(1);
				}
			
				memset(host, 0, sizeof(host));
				if(getnameinfo((struct sockaddr*)&sin[j], fromlen, host, sizeof(host), 0, 0, 0) != 0){
					perror("getnameinfo");
					exit(2);
				}
				
				strcat(host, "\n");
				write(fd, host, strlen(host));
				close(acc[j]);
			}
		}
		i++;	
	}
	
	close(fd);
	
	for(i=0; i<argc; i++)
		close(sock[i]);
		
	return EXIT_SUCCESS;
}

/*

Select:
-------

1) Créer et binder un ensemble de sockets + listen
2) Initialiser l'ensemble vide de descripteur (FD_ZERO(fd_set))
3) Remplir l'ensemble de descripteur avec FD_SET
4) Select (bloquant)
5) Au retour de select (Attention: selec modifie ses paramètres pour indiquer les descripteurs en attente)
6) Tester fd_set FD_ISSET (Attention: select ne lit pas les données)
7) Accept (->boucle sur la 2))

*/
