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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct sockaddr_in sin;
int sock, port, addr, acc;
char *rep;

int main(int argc, char **argv){
	char c;
	char val[1024];
	int p;
	
	if(argc != 3){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	addr = atoi(argv[1]);
	port = atoi(argv[2]);
	
	memset(&sin, '\0', sizeof(struct sockaddr_in));
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Erreur creation socket");
		exit(1);
	}
	
	if(connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1){
		perror("connect"); 
		exit(1);
	}
	
	while((c = getchar())){
		write(sock, &c, sizeof(char));
		
		if(c == '\n'){
			read(sock,&p,sizeof(int));;
			
			if(p == 1)
				break;
			
			if(p == 2)
				continue;
			
			read(sock,val,1024*sizeof(char));
			
			if(val == "")
				break;
			
			printf("La valeur retourné par GET = %s\n",val);
		}
	}
	
	shutdown(sock, 2);
	close(sock);

	return EXIT_SUCCESS;	
}
