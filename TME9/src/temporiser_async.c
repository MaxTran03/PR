#define _XOPEN_SOURCE 700 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <aio.h>
#include <time.h>
#include <errno.h>

void sig_handler(int sig, siginfo_t *si, void *uc){}

int main(int argc, char **argv){
	int fd1, fd2, mysig;
	char c;
	struct aiocb a;
	sigset_t sig;
	timer_t timerid;
	struct sigaction action;
	struct sigevent sigspec;
	struct itimerspec new, old;
	
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
	sigspec.sigev_notify = SIGEV_SIGNAL;
	sigspec.sigev_signo = SIGRTMIN;
	sigspec.sigev_value.sival_ptr = &timerid;
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
	a.aio_sigevent.sigev_notify = SIGEV_NONE;
	
	aio_write(&a);
	
	if((fd2 = open(argv[1], O_RDWR, 0600)) == -1){
		perror("open");
		return EXIT_FAILURE;
	}
	
	/* Temporisateur */
	timer_create(CLOCK_REALTIME, &sigspec, &timerid);
	
	new.it_value.tv_sec = 2;
	new.it_value.tv_nsec = 0;
	new.it_interval.tv_sec = 0;
	new.it_interval.tv_nsec = 50;
	
	timer_settime(timerid, 0, &new, NULL);
	
	while(sigwait(&sig, &mysig)){
		if(aio_error(&a) == 0)
			break;
	}
	
	printf("Affichage: ");
	while(read(fd2, &c, sizeof(char))){
		printf("%c", c);
	}
	
	printf("\n\n##### Fin de programme #####\n");
	
	close(fd1);
	close(fd2);
	timer_delete(timerid);
	
	return EXIT_SUCCESS;
}

/*

1) Configuration signal à envoyer par le timer (struct sigevent)
2) Création timer avec timer_create (timer_t)
3) Configuration délai timer (struct itimerspec)
4) Armer le timer avec timer_settime
5) Attendre les signaux
6) Détruire le timer (timer_delete)

*/
