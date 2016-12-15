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
#include <netinet/ip.h>
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
struct sockaddr_in sin, exp;
char message[80];
char messagercv[80];
char ident[80];
int sock;
int broadcast = 1;


void func_thread_wait(){
	printf("##### Début WAIT #####\n");
	int fromlen = sizeof(exp);
	while(1){
		if(recvfrom(sock, messagercv, sizeof(messagercv), 0, (struct sockaddr *)&exp, (socklen_t*)&fromlen) == -1){
			perror("recvfrom");
			exit(1);
		}
		
		if(strcmp(messagercv, "PONG") == 0){
			printf("MESSAGE RECU : %s\n",messagercv);
			if (getnameinfo((struct sockaddr*)&sin, sizeof(sin), ident, sizeof(ident), 0, 0, 0) != 0) {
				perror("getnameinfo"); 
				exit(1);
			}
			printf("L'identité du pong est %s\n\n", ident);
		}
	}
}

void func_thread_broadcast(){
	printf("##### Début BROADCAST #####\n");
	
	strcpy(message, "PING");
	while(1){
		if(sendto(sock, message, strlen(message)+1, 0, (struct sockaddr *)&sin, sizeof(sin)) == -1){
			perror("sendto");
			exit(1);
		} 
	
		printf("Message envoyé!\n");
		sleep(3);
	}
}

int main(){
	pthread_t tid1, tid2;
	
	printf("##### Début Programme #####\n");
	
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("Erreur creation socket");
		exit(1);
	}
	
	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1){
		perror("setsockopt");
		exit(1);
	}
	
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &broadcast, sizeof(broadcast)) == -1){
		perror("setsockopt");
		exit(1);
	}
	
	
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	
	if(bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1){
		perror("Erreur au nommage");
		exit(1);
	}
	
	pthread_create(&tid1, NULL, func_thread_wait, NULL);
	pthread_create(&tid2, NULL, func_thread_broadcast, NULL);
	
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	
	close(sock);
	
	printf("##### Fin Programme #####\n");
	
	return 0;
}

