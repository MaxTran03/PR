#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

int N;
int *sp;

void remonte_ipc(){
	int i;
	printf("Début fonction\n");

	
	for(i=0; i<N; i++){
		if(fork() == 0){
			srand(getpid());
			sp[i] = (int) (10*(float)rand()/ RAND_MAX);
			printf("Le fils a choisi %d\n", sp[i]);
			exit(0);
		}
	}
	
	for(i=0; i<N; i++)
		wait(NULL);
	
	int somme = 0;
	
	for(i=0; i<N; i++){
		somme += sp[i];
	}
	
	printf("La somme des valeurs est: %d\n", somme);
	printf("Fin fonction\n");
}


int main(int argc, char *argv[]){
	int shm_fd;
	
	if(argc != 2){
		perror("Error argument\n");
		return EXIT_FAILURE;
	}
	
	N = atoi(argv[1]);
	
	/* Creation du segment monshm */
	if((shm_fd = shm_open("monshm", O_RDWR|O_CREAT, 0600)) == -1){
		perror("shm_open");
		exit(1);
	}
	
	/* Allouer au segment une taille */
	if(ftruncate(shm_fd, N*sizeof(int)) == -1){
		perror("ftruncate");
		exit(1);
	}
	
	/* Mapper le segment */
	if((sp = mmap(NULL, N*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	
	remonte_ipc();
	
	/* Détachement du segment */
	munmap(sp, sizeof(int));
	
	/* Destruction du segment */
	shm_unlink("monshm");
	
	return EXIT_SUCCESS;
}




