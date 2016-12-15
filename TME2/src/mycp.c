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


int main(int argc, char *argv[]){

	if(argc != 3){
		printf("pas le bon nombre d'argument\n");
		return EXIT_FAILURE;
	}
	
	int fd1, fd2;
	
	if ((fd1 = open (argv[1], O_RDONLY, 0600)) == -1)
        return EXIT_FAILURE;
        
	if ((fd2 = open (argv[2], O_RDWR| O_CREAT | O_TRUNC, 0600)) == -1)
        return EXIT_FAILURE;
	
	int n;
	char c;
	
	while((n = read(fd1, &c, sizeof(char)))){
		if (n == -1)
			return EXIT_FAILURE;
		
		if((n = write(fd2, &c, sizeof(char))) == -1)
			return EXIT_FAILURE;
	}
	
	close(fd1);
	close(fd2);
	
	return 0;
	
}
