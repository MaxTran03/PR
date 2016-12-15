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


int main(){

	int i;
	FILE * pFile;
	char c;
	
	if ((pFile = fopen ("f2", "r")) == NULL)
        return EXIT_FAILURE;
	
	for(i=0; i<3; i++){
		if(fork() == 0){
		 printf("allo%d\n",getpid());
			while((c = fgetc(pFile)) != EOF){
	 			printf("Caractère lu %c pid %d\n", c, getpid());
			}
			 printf("quit%d\n",getpid());
			exit(1);
	 	}
	}
	fclose(pFile);
	return 0;
}



































