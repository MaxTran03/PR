#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

int N, mysig, cpt = 0;
union sigval val;
sigset_t block_mask;
struct sigaction action;
pid_t pid;
pid_t *tabpid;

void sig_handler(int signo, siginfo_t *si, void *context){
	tabpid[cpt] = si->si_pid;
	cpt++;
	sigqueue(getpid(), SIGRTMIN, val);
}

void process(int n){
	printf("avant barrière\n");
	wait_barriere(n);
	printf("après barrière\n");
	exit(0);
}

void wait_barriere(int n){
	int i;
	if(n == 0){
		for(i=1; i<N; i++)
			sigwait(&block_mask, &mysig);
		for(i=1; i<N; i++)
			sigqueue(tabpid[i-1], SIGRTMIN+1, val);
		
	}else{
		sigqueue(pid, SIGRTMIN+2, val);
		sigwait(&block_mask, &mysig);
	}
}

int main(int argc, char **argv){
	int i;
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	N = atoi(argv[1]);
	tabpid = (char*)malloc((N-1)*sizeof(int));
	
	action.sa_sigaction = sig_handler;
	action.sa_flags = SA_SIGINFO;
	
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGRTMIN);
	sigaddset(&block_mask, SIGRTMIN+1);
	sigaction(SIGRTMIN+2, &action, NULL);
	sigprocmask(SIG_SETMASK, &block_mask, NULL);
	
	
	for(i=0; i<N; i++){
		if(i == 0){
			if((pid = fork()) == 0)
				process(i);
		}else if(fork() == 0){
			process(i);
		}
	}
	
	for(i=0; i<N; i++)
		wait(NULL);
	
	free(tabpid);
	
	return EXIT_SUCCESS;
}
