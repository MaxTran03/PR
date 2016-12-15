#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>


int N, C, P, M;
int shm_fd;
sem_t *baguettes;


void diner(int n, int c, int p, int m){
	int i;
	
	for(i=0; i<c; i++){
		/* Etape 1 */
		printf("Le philosophe %d pense\n", n);
		sleep(p);
		
		/* Etape 2 */
		if(n%2 == 0){
			sem_wait(&baguettes[n%N]);
			sem_wait(&baguettes[(n+1)%N]);
		}else{
			sem_wait(&baguettes[(n+1)%N]);
			sem_wait(&baguettes[n%N]);
		}
		
		printf("-> Le philosophe %d mange\n", n);
		sleep(m);
		
		/* Etape 3 */
		sem_post(&baguettes[n%N]);
		sem_post(&baguettes[(n+1)%N]);
		printf("-----> Le philosophe %d a déposé la baguette\n", n);
	}	
	exit(0);
}

int main(int argc, char *argv[]){
	int i;
	
	/* Nombre de philosophes et de baguettes */
	N = atoi(argv[1]);
	/* Nombre de cycle */
	C = atoi(argv[2]);
	/* Penser pendant secondes */
	P = atoi(argv[3]);
	/* Manger pendant secondes */
	M = atoi(argv[4]);
	
	if(argc != 5){
		perror("Error argument\n");
		return EXIT_FAILURE;
	}

	if((shm_fd = shm_open("monshm", O_RDWR|O_CREAT, 0600)) == -1){
		perror("shm_open");
		exit(1);
	}
	

	if(ftruncate(shm_fd, N*sizeof(sem_t)) == -1){
		perror("ftruncate");
		exit(1);
	}
	

	if((baguettes = mmap(NULL, N*sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	
	
	for(i=0; i<N; i++)
		sem_init(&baguettes[i], 1, 1);
	
	for(i=0; i<N; i++){
		if(fork() == 0)
			diner(i, C, P, M);
	}
	
	for(i=0; i<N; i++)
		wait(NULL);

	sem_close(baguettes);

	sem_unlink("monsem");
	
	munmap(baguettes, N*sizeof(sem_t));
	sem_unlink("monshm");
	return EXIT_SUCCESS;
}
