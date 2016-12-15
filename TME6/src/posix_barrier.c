#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>

int N, *cpt;
sem_t *sem;

void process (int NB_PCS) {
	printf ("avant barrière\n");
	wait_barrier (NB_PCS);
	printf ("après barrière\n");
	exit (0);
}

void wait_barrier (int NB_PCS){
	int i;
	if(*cpt == NB_PCS-1){
		for(i=0; i<NB_PCS-1; i++){
			sem_post(sem);
		}
	}else{
		(*cpt)++;
		sem_wait(sem);
	}
}


int main(int argc, char *argv[]){
	int i;
	
	if(argc != 2){
		perror("Erreur argument\n");
		return EXIT_FAILURE;
	}

	N = atoi(argv[1]);
	
	sem = sem_open("monsem", O_CREAT|O_EXCL|O_RDWR, 0666, 0);
	
	int fd = shm_open("monshm", O_RDWR|O_CREAT, 0600);
	ftruncate(fd, sizeof(int));
	cpt = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	*cpt=0;
	
	for(i=0; i<N; i++)
		if(fork()==0)
			process(N);
			
	for(i=0; i<N; i++)
		wait(NULL);
	
	
	/* Fermeture des sémaphores */
	sem_close(sem);
	
	/* Destruction des sémaphores */
	sem_unlink("monsem");
	
	return EXIT_SUCCESS;
}

/* Avec Compteur partagé:
	- Mémoire partagée pour le compteur
	- Semaphore Mutex (init 1)
	- Semaphore Synchro (init 0)
	
	Sans Compteur partagé:
	- Semaphore Synchro1 (init 0)
	- Semaphore Synchro2 (init 0)
	
	barriere(chef?){
		if(chef){ // un processus different des autres
			for(i=0; i<(N-1); i++)
				P(Synchro1);
			for(i=0; i<(N-1); i++)
				V(Synchro2);
		}else{
			V(Synchro1);
			P(Synchro2);
		}
	}

*/
