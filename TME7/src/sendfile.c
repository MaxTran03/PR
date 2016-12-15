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

struct sockaddr_in sin;
int sock, port, addr, acc;
char *rep;

int main(int argc, char **argv){
	char c;
	int n, fd;
	
	if(argc != 4){
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
	
	write(sock, argv[3], sizeof(argv[3]));
	fd = open(argv[3], O_RDONLY, 0600);
	
	while((n = read(fd, &c, sizeof(char)))){
		write(sock, &c, sizeof(char));
	}
	
	shutdown(sock, 2);
	close(sock);
	close(fd);

	return EXIT_SUCCESS;	
}
