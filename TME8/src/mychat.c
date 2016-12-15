#define _POSIX_SOURCE 1

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
#include <sys/mman.h>

struct ip_mreq {
	struct in_addr imr_multiaddr;
	struct in_addr imr_interface;
};

char pseudo[80];
char pseudo2[80];

struct ip_mreq imr;
struct sockaddr_in sin;
char message[80];
char messagercv[80];
int sock;


void func_thread_read(){
	strcpy(pseudo2, pseudo);
	strcat(pseudo2, " ");
	while(1){
		if(recv(sock, messagercv, sizeof(messagercv), 0) == -1){
			perror("recvfrom");
			exit(1);
		}
		
		if(strncmp(messagercv, pseudo2, strlen(pseudo2)))
			printf("%s\n", messagercv);
	}
}

void func_thread_write(){
	char c;
	while(1){
		strcpy(message, pseudo);
		strcat(message, " : ");
		while((c = getchar()) != '\n'){
			strcat(message, &c);
		}
		if(sendto(sock, message, strlen(message)+1, 0, (struct sockaddr *)&sin, sizeof(sin)) == -1){
			perror("sendto");
			exit(1);
		} 
	}
}

int main(int argc, char *argv[]){
	pthread_t tid1, tid2;
	int fd;
	
	if(argc != 4){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	strcpy(pseudo, argv[3]);
	
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("Erreur creation socket");
		exit(1);
	}
	
	imr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	imr.imr_interface.s_addr = htonl(INADDR_ANY);
	
	if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&imr, sizeof(struct ip_mreq)) == -1){
		perror("setsockopt1");
		exit(1);
	}
	
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&imr, sizeof(struct ip_mreq)) == -1){
		perror("setsockopt1");
		exit(1);
	}
	
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(atoi(argv[2]));
	
	if(bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1){
		perror("Erreur au nommage");
		exit(1);
	}

	pthread_create(&tid1, NULL, func_thread_read, NULL);
	pthread_create(&tid2, NULL, func_thread_write, NULL);
	
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	
	close(sock);
	
	printf("##### Fin Programme #####\n");
	
	return 0;
}
