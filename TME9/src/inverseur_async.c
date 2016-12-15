#define _POSIX_SOURCE 1 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>

int main(int argc, char *argv[]){
	int fd1, fd2, i,n,res;
	char buf[10];
	struct aiocb a, *list[1]={&a};
	
	if(argc != 3){
		perror("Error argument!");
		return EXIT_FAILURE;
	}
	
	if((fd1 = open(argv[1], O_RDWR, 0600)) == -1){
		perror("open");
		return EXIT_FAILURE;
	}
	
	if((fd2 = open(argv[2], O_CREAT|O_TRUNC|O_WRONLY, 0600)) == -1){
		perror("open");
		return EXIT_FAILURE;
	}
	
	a.aio_fildes = fd2;
	a.aio_nbytes = sizeof(char);
	a.aio_offset = 0;
	a.aio_reqprio = 0;
	a.aio_sigevent.sigev_notify = SIGEV_NONE;
	
	lio_listio(LIO_NOWAIT, list, 1, NULL);
	
	while(n = read(fd1, buf, 10*sizeof(char))){
		for(i=n-1; 0<=i; i--){
			a.aio_buf = &buf[i];
			aio_write(&a);
			a.aio_offset += a.aio_nbytes;
			while((res = aio_error(&a)) == EINPROGRESS){
				res = aio_suspend(list, 1, NULL);
				if(res == -1)
					return errno;
			}
		}
	}

	close(fd1);
	close(fd2);
	
	return EXIT_SUCCESS;
}
