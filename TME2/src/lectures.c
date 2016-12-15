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


int func1(char *fic){

	int i, n;
	int fd;
	char c;
	
	if ((fd = open (fic, O_RDWR, 0600)) == -1)
        return EXIT_FAILURE;
	
	for(i=0; i<3; i++){
		if(fork() == 0){
			while((n = read(fd, &c, sizeof(char)))){
	 			if(n == -1)
	 				return EXIT_FAILURE;
	 			printf("Caractère lu %c pid %d\n", c, getpid());
			}
			exit(1);
	 	}
	}
	
	for(i=0; i<3; i++)
		wait(NULL);
		
	return 0;
}

int func2(char *fic){

	int i;
	FILE * pFile;
	char c;
	
	if ((pFile = fopen (fic, "r")) == NULL)
        return EXIT_FAILURE;
	
	for(i=0; i<3; i++){
		if(fork() == 0){
			while((c = fgetc(pFile)) != EOF){
				setvbuf(pFile, &c, _IONBF, 1);
	 			printf("Caractère lu %c pid %d\n", c, getpid());
			}
			exit(1);
	 	}
	}
	
	for(i=0; i<3; i++)
		wait(NULL);
	
	fclose(pFile);
	
	return 0;
}

int main(int argc, char *argv[]){
	
	if(argc != 2){
		printf("Il faut un argument!\n");	
		return -1;
	}
	
	/*printf("---- Début func1 ----\n"); 
	func1(argv[1]);
	printf("---- Fin func1 ----\n");*/
	
	printf("---- Début func2 ----\n"); 
	func2(argv[1]);
	printf("---- Fin func2 ----\n");
	
	return 0;
}

/* On remarque qu'avec les standard POSIX open et read, on a une meilleure commutation de processus. Alors qu'avec fopen et fgetc, on a un processus pour les 3 fork. 



*/






























