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

struct sockaddr_in sin;
int sock, port, addr, acc;
char rep[1024];

int main(int argc, char **argv){
	int fromlen;
	
	if(argc != 2){
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
	fromlen = sizeof(sin);
	
	while(1){
		if((acc = accept(sock, (struct sockaddr *)&sin, &fromlen)) == -1){
			perror("accept");
			exit(1);
		}

		char *c;
		int n;
	
		strcpy(rep, "");
		char id[1024];
		setenv(id, "", 1);
		
		while((n = read(acc, c, 1))){
			strcat(rep, c);
			char *value = "SET";
			int *val2;
			
			if(c[0] == '\n'){
				printf("Le client a tapé la commande : %s\n",rep);
				
				if(rep[0] == 'S'){
					int i=2;
					strcpy(id,"");
					
					while((rep[i]>='a' && rep[i]<='z')||(rep[i]>='A' && rep[i]<='Z')){
						strncat(id,&rep[i],1);
						i++;
					}

					i++;
					char val[1024];
					strcpy(val, "");
					while(rep[i] != '\n'){
						strncat(val, &rep[i], 1);
						i++;
					}

					setenv(id,val,1);
					
					int c = 2;
					write(acc, &c, sizeof(int));
				}
				
				if(rep[0] == 'G'){
					int i = 2;
					strcpy(id, "");
					
					while(rep[i] != '\n'){
						strncat(id, &rep[i], 1);
						i++;
					}
					
					val2 = getenv(id);
					printf("GET %s = %s\n", id, val2);
					int c = 0; 
					write(acc, &c, sizeof(int));
					write(acc, val2, 1024);
				}
				
				if(rep[0] == 'Q'){
					setenv(id,"",1);
					printf("Le client quitte\n");
					value = "";
					int c = 1;
					write(acc, &c, sizeof(int));
					close(acc);
					break;
				}
				
				strcpy(rep, "");
			}
		}
	}
	
	close(acc);
	shutdown(sock, 1);
	close(sock);
	
	return EXIT_SUCCESS;
}
