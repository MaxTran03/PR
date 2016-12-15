#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>


void sig_handler(int signo, siginfo_t *s, void* c){}

int main(int argc, char *argv[]){
	int N,i,mysig;
	union sigval val;
	sigset_t block_mask;
	struct sigaction action;
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	N = atoi(argv[1]);
	action.sa_sigaction = sig_handler;
	action.sa_flags = SA_SIGINFO;
	action.sa_mask = block_mask;
	sigemptyset(&block_mask);
	sigaddset(&block_mask,SIGRTMIN);
	sigaction(SIGRTMIN, &action, NULL);
	sigprocmask(SIG_SETMASK, &block_mask, NULL);
	
	printf("papa %d\n", getpid());
	
	for(i=0;i<N;i++){
		if(fork()==0){
			if(i == N-1){
				sigqueue(getppid(),SIGRTMIN,val);
				printf("Fils numero %d de pid %d ppid %d\n",i+1,getpid(), getppid());
				exit(1);
			}
		}else{
			sigwait(&block_mask, &mysig);
			printf("sdqsdFils numero %d de pid %d ppid %d\n",i,getpid(), getppid());
			if (i!=0)
				sigqueue(getppid(),SIGRTMIN,val);
			
			break;
		}
	}

	return EXIT_SUCCESS;
}
