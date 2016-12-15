#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

void sig_hand(int sig){}

int main(){
	pid_t pid, pid_pere = getpid();
	sigset_t sig_proc;
	struct sigaction action;
	
	sigfillset(&sig_proc);
	sigdelset(&sig_proc, SIGUSR1);
	sigdelset(&sig_proc, SIGUSR2);
	
	action.sa_mask = sig_proc;
	action.sa_flags = 0 ;
	action.sa_handler = sig_hand;
	
	sigaction(SIGUSR1, &action, 0);
	sigaction(SIGUSR2, &action, 0);
	
	if((pid = fork())==0){
		wait(NULL);
		kill(pid_pere, SIGUSR2);
		
		if(fork()==0){
			kill(pid_pere, SIGUSR1);
		}
	}else{
		sigsuspend(&sig_proc);
		printf("Processus P3 créé\n");
		sigsuspend(&sig_proc);
		printf("Processus P3 terminé\n");
		wait(NULL);
		printf("Processus P2 terminé\n");
	}	
	
	return 0;
}
