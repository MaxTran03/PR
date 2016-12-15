#define _XOPEN_SOURCE 700
#define _REENTRANT

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int cpt = 0;
int NBTHREAD;
int NBFILE;

char **argv2;

int upper(char *path){
	FILE* fp1, *fp2;
	int c = 1;
	
	fp1 = fopen (path, "r");
	fp2 = fopen (path, "r+");
	if((fp1== NULL) || (fp2== NULL)){
		perror ("fopen");
		return errno;
	}
		
	while(c!=EOF) {
		c = fgetc(fp1);
		if(c!=EOF)
			fputc(toupper(c),fp2);
	}

	fclose(fp1);
	fclose(fp2);

	return EXIT_SUCCESS;
}

void *thread_func(char *arg){
	pthread_mutex_lock(&mutex);
	while(cpt != NBFILE){
		char *fic = argv2[cpt+2];
		cpt++;
		printf("%d\n",cpt);
		pthread_mutex_unlock(&mutex);
		upper(fic);
	}
	
	pthread_exit ((void*)0);
}


int main(int argc, char*  argv[]){
	int i;
	NBTHREAD = atoi(argv[1]);
	NBFILE = argc-2;
	argv2 = argv;
	
	pthread_t tid[NBTHREAD];
	
	for(i=0; i<NBTHREAD; i++){
		pthread_create(&tid[i], NULL, thread_func, argv);
	}
	
	for(i=0; i<NBTHREAD; i++)
		pthread_join(tid[i], NULL);

	pthread_mutex_destroy(&mutex);

	return EXIT_SUCCESS;
}
