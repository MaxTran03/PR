#define _XOPEN_SOUCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>


int main(int argc, char * argv[]){
	
	int fd1, fd2, i=-2;
	char c;
	
	if ((fd1 = open (argv[1], O_RDONLY, 0600)) == -1)
		return EXIT_FAILURE;
		
	if ((fd2 = open (argv[2], O_RDWR|O_TRUNC|O_CREAT, 0600)) == -1)
		return EXIT_FAILURE;
	
	while((lseek(fd1, i, SEEK_END) >= 0) && read(fd1, &c, sizeof(char))){
		if(write(fd2, &c, sizeof(char)) == -1){
			perror("write");
			return EXIT_FAILURE;
		}
		
		i--;
	}
	
	close(fd1);
	close(fd2);
	
	return 0;
}




