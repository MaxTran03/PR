#define _XOPEN_SOURCE 700 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>

int somme = 0;

void interrupt_signal(int signo, siginfo_t *si, void *context){
	printf("Valeur associée %d - %d\n", si->si_pid, si->si_value.sival_int);
	somme += si->si_value.sival_int;
}

int main(int argc, char *argv[]){
	int mysig, i, N, alea;
	union sigval val;
	sigset_t block_mask;
	struct sigaction action;
	
	printf("##### Début de programme #####\n\n");
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	N = atoi(argv[1]);
	
	printf("LE PERE DE TOUS %d\n", getpid());
	
	action.sa_sigaction = interrupt_signal;
	action.sa_flags = SA_SIGINFO;
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGRTMIN);
	action.sa_mask = block_mask;
	sigaction(SIGRTMIN, &action, 0);
	

	for(i=0; i<N; i++){
		if(fork() == 0){
			srand(getpid());
			alea = (int)(10*(float)rand()/ RAND_MAX);
			
			printf("Le fils %d de pid %d a choisi %d\n", i+1, getpid(), alea);
			
			val.sival_int = alea;
			
			if(sigqueue(getppid(), SIGRTMIN, val) == -1){
				perror("sigqueue");
				return EXIT_FAILURE;
			}
			
			return EXIT_SUCCESS;
		}
	}
	
	for(i=0; i<N; i++)
		wait(NULL);
	
	printf("La somme est : %d", somme);
	
	printf("\n\n##### Fin de programme #####\n");
	
	return EXIT_SUCCESS;
}



