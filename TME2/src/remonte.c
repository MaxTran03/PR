#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define N 10

int main(int argc, char *argv[]){
	int n1;
	int i, fd;
	
	
	if ((fd = open ("tmp", O_RDWR| O_CREAT | O_TRUNC, 0600)) == -1)
        return EXIT_FAILURE;
	
 	for(i=0; i<N; i++){
	 	if (fork() == 0){
	 		srand(getpid());
	 		int nb = (int) (10*(float)rand() / RAND_MAX);
	 		if((n1 = write(fd, &nb, sizeof(int))) == -1)
				return EXIT_FAILURE;
			return EXIT_SUCCESS;
	 	}
 	}
 	
 	int j;
	for(j=0; j<N; j++)
	 	wait(NULL);
	 	
	int nb2, n2;
	int val = 0;
	int cpt = 1;
	
	if (lseek (fd, 0, SEEK_SET) == -1)
   	return EXIT_FAILURE;
   	
	while((n2 = read(fd, &nb2, sizeof(int)))){
		if(n2 == -1)
	 		return EXIT_FAILURE;
	 	printf("Element %d : %d\n", cpt, nb2); 
	 	val += nb2;
	 	cpt++;
	}
	
	printf("La somme des valeurs aléatoires est égale à : %d\n", val);
	
	close(fd);
	
	return 0;
	
}
