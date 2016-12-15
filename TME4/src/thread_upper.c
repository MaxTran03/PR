#define _XOPEN_SOURCE 700
#define _REENTRANT

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>

int upper(char *path){
	FILE* fp1, *fp2;
	int c = 1;

	fp1= fopen (path, "r");
	fp2= fopen (path, "r+");
	if ((fp1== NULL) || (fp2== NULL)) {
		perror ("fopen");
		return errno;
	}

	while (c !=EOF) {
		c=fgetc(fp1);
		if (c!=EOF)
			fputc(toupper(c),fp2);
	}

	fclose (fp1);
	fclose (fp2);

	return EXIT_SUCCESS;
}

int main(int argc, char*  argv[]){
	
	int i;
	pthread_t tid[argc-1];
	
	for(i=1;i<argc;i++)
		pthread_create(&tid[i-1],NULL,upper,argv[i]);
		
	for(i=1;i<argc;i++)
		pthread_join(tid[i-1],NULL);


	return 0;
}
