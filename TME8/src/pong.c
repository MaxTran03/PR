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
#include <sys/select.h>
#include <pthread.h>

#define PORT 9999

struct ip_mreq {
	struct in_addr imr_multiaddr;
	struct in_addr imr_interface;
};

struct ip_mreq imr;

int main(){
	struct sockaddr_in dest, exp;
	int sock;
	char message[80];
	int fromlen = sizeof(exp);
	int broadcast = 1;
	
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("socket");
		exit(1);
	}
	
	memset((char *)&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(PORT);
	dest.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	
	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1){
		perror("setsockopt");
		exit(1);
	}
	
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &broadcast, sizeof(broadcast)) == -1){
		perror("setsockopt");
		exit(1);
	}
	
	if(bind(sock, (struct sockaddr *)&dest, sizeof(dest)) == -1){
		perror("Erreur au nommage");
		exit(1);
	}
	
	while(1){
		if(recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *)&exp, (socklen_t*)&fromlen) == -1){
			perror("recvfrom");
			exit(1);
		}
		
		if(strncmp(message, "PING",4) == 0){
			printf("MESSAGE RECU : %s\n",message);
			strcpy(message, "PONG");
			if(sendto(sock, message, sizeof(message)+1, 0, (struct sockaddr*)&dest, sizeof(dest)) == -1){
				perror("sendto");
				exit(1);
			}
			printf("Pong envoyé!\n\n");
		}
	}
	
	memset(message,0,sizeof(message));
	close(sock);
	
	return 0;
}
