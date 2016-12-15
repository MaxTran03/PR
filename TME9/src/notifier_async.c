#define _XOPEN_SOURCE 700 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <aio.h>

void sig_handler(){}

int main(int argc, char **argv){
	int fd1, fd2, mysig;
	char c;
	struct aiocb a;
	sigset_t sig;
	struct sigaction action;
	
	printf("##### Début de programme #####\n\n");
	
	if(argc != 3){
		perror("Error argument!");
		return EXIT_FAILURE;
	}
	
	action.sa_sigaction = sig_handler;
	action.sa_flags = SA_SIGINFO;
	action.sa_mask = sig;
	sigemptyset(&sig);
	sigaddset(&sig, SIGRTMIN);
	sigaction(SIGRTMIN, &action, 0);
	sigprocmask(SIG_SETMASK, &sig, 0);
	
	if((fd1 = open(argv[1], O_CREAT|O_TRUNC|O_WRONLY, 0600)) == -1){
		perror("open");
		return EXIT_FAILURE;
	}
	
	a.aio_fildes = fd1;
	a.aio_buf = argv[2];
	a.aio_nbytes = strlen(argv[2]);
	a.aio_offset = 0;
	a.aio_reqprio = 0;
	a.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	a.aio_sigevent.sigev_signo = SIGRTMIN;
	
	aio_write(&a);
	
	if((fd2 = open(argv[1], O_RDWR, 0600)) == -1){
		perror("open");
		return EXIT_FAILURE;
	}
	
	/* Attente de fin I/O */
	/*sigdelset(&sig, SIGRTMIN);
	sigsuspend(&sig);*/
	
	if(sigwait(&sig, &mysig) == -1){
		perror("sigwait");
		return EXIT_FAILURE;
	}
	
	printf("Affichage: ");
	while(read(fd2, &c, sizeof(char))){
		printf("%c", c);
	}
	
	printf("\n\n##### Fin de programme #####\n");
	
	close(fd1);
	close(fd2);
	
	return EXIT_SUCCESS;
}

/*

1) Configurer aiocb (notification par signal)
2) Masquer signal de notification
3) aio_write
4) Attente de signal (sigwait);

*/
