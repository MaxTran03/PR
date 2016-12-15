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

int main(int argc, char **argv){
	int fd1, fd2, res;
	char *c;
	struct aiocb a, *list[1]={&a};
	
	printf("##### Début de programme #####\n\n");
	
	if(argc != 3){
		perror("Error argument!");
		return EXIT_FAILURE;
	}
	
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
	
	while((res = aio_error(&a)) == EINPROGRESS){
		printf("Toujours en cours d'exécution...\n");
	
		/* Attente de fin I/O */
		res = aio_suspend(list, 1, NULL);
		if(res == -1)
			return errno;
	}
	
	aio_return(&a);
	
	aio_read(&a);
	
	printf("Affichage: %s", a.aio_buf);
	
	printf("\n\n##### Fin de programme #####\n");
	
	close(fd1);
	close(fd2);
	
	return EXIT_SUCCESS;
}

/*

1) Configurer aiocb (pas de notification)
2) aio_write
3) aio_suspend

Attention: test avec aio_error + boucle !!!

*/
