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
	
	int i;
	pid_t pid;
	pid_t tab[10];
	sigset_t sig_proc;
	struct sigaction action;
	
	sigfillset(&sig_proc);
	sigdelset(&sig_proc, SIGUSR1);
	sigdelset(&sig_proc, SIGINT);
	
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;
	
	sigaction(SIGUSR1, &action, 0);
	
	for(i=0;i<10;i++){
		if ((pid=fork())!=0){
			sigsuspend(&sig_proc);
			printf("mon papa %d, moi %d, mon fils %d\n",getppid(),getpid(),pid);
			if (i!=0)
				kill(getppid(),SIGUSR1);
			return 0;		
		}else{
			tab[i] = getppid();
			if(i==9){
				int j;
				for(j=0;j<10;j++){
					printf("pid %d : %d \n", j, tab[j]);
				}
				kill(getppid(),SIGUSR1);
			}			
		}		
	}
	
	return 0;
}
