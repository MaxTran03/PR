/**** max_calcul.c ****/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char* argv[]){
	int i;
	
	if(argc==1){
		printf("Le vecteur est nul!\n");
		exit(1);	
	}
	
	int max = atoi(argv[0]);
	
	for(i=2; i<argc; i++){
		if(atoi(argv[i]) > max)
			max = atoi(argv[i]);
	}
	
	printf("Le maximum est : %d\n", max);
	
	return 0;

}
