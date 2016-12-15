#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

void sig_hand(int sig){
	if(sig == SIGINT)
		printf("Signal reçu!\n");
}

int main(){
	int i;
	pid_t pid;
	pid_t principal = getpid();
	sigset_t sig_proc;
	struct sigaction action;
	
	sigfillset(&sig_proc);
	sigdelset(&sig_proc, SIGINT);
	
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;
	
	sigaction(SIGINT, &action, 0);
	sigprocmask(SIG_SETMASK, &sig_proc, NULL);
	
	
	for(i=0;i<10;i++){
		if((pid = fork()) == 0){
			if(i == 9){
				printf("Processus %d de pid %d s'est stoppé\n", 10, getpid());
				kill(principal, SIGINT);
				kill(getpid(), SIGSTOP);
				sleep(1);
				kill(principal, SIGINT);
				printf("Fin du processus 10 de pid %d\n", getpid());
			}
		}else {
			if(i == 0){
				sigsuspend(&sig_proc);
				printf("Tous les descendants sont stoppés, je suis le principal de pid %d\n", getpid());
				kill(pid, SIGCONT);
				sigsuspend(&sig_proc);
				printf("Fin du programme, je suis le principal de pid %d\n", getpid());
				break;
			}else if(i <= 9){
				printf("Processus %d de pid %d s'est stoppé\n", i, getpid());
				kill(getpid(), SIGSTOP);
				kill(pid, SIGCONT);	
				printf("Fin du processus %d de pid %d\n", i, getpid());
				return 0;
			}
			
		}
		
	}
	
	
	
	
	return 0;
	
}
