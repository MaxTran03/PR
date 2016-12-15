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

#define SIZE 4 

int main(int argc, char *argv[]){
	int fd1, i, N, alea, res, somme = 0;
	char buf[SIZE];
	struct aiocb a, *list[1] = {&a};
	
	printf("##### Début de programme #####\n\n");
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}
	
	if((fd1 = open("fic", O_CREAT|O_TRUNC|O_RDWR, 0600)) == -1){
		perror("open");
		return EXIT_FAILURE;
	}
	
	N = atoi(argv[1]);

	memset(buf, '\0', SIZE);
	a.aio_fildes = fd1;
	a.aio_buf = buf;
	a.aio_nbytes = SIZE;
	a.aio_reqprio = 0;

	for(i=0; i<N; i++){
		if(fork() == 0){
			srand(getpid());
			alea = (int)(10*(float)rand()/ RAND_MAX);
			sprintf(buf, "%d", alea);
			
			printf("Le fils %d a choisi %d\n", i+1, alea);
			
			a.aio_offset = i * SIZE;
			a.aio_lio_opcode = LIO_WRITE;
			
			aio_write(&a);
			
			res = aio_error(&a);
			while(res == EINPROGRESS){
				printf("Fils %d : Toujours en cours d'exécution...\n", i+1);
				/* Attente de fin I/O */
				res = aio_suspend(list, 1, NULL);
				if(res == -1)
					return errno;
			}
			aio_return(&a);
			
			printf("Fin d'exécution du fils %d\n", i+1);
			return EXIT_SUCCESS;
		}
	}
	
	for(i=0; i<N; i++)
		wait(NULL);
	
	a.aio_lio_opcode = LIO_READ;
	
	for(i=0; i<N; i++){
		a.aio_offset = i * SIZE;
		aio_read(&a);
		
		res = aio_error(&a);
		while(res == EINPROGRESS){
			res = aio_suspend(list, 1, NULL);
			if(res == -1)
				return errno;
		}
		aio_return(&a);
		
		printf("Affichage: %s\n", a.aio_buf);
		somme += atoi(a.aio_buf);
		
	}
	
	printf("La somme est : %d\n", somme);
	
	printf("\n\n##### Fin de programme #####\n");
	
	close(fd1);
	
	return EXIT_SUCCESS;
}



