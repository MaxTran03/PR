/**** max_func.c ****/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "max_func.h"

int max_func(int *vect, int nb){
	int i;
	
	if(nb==0){
		printf("Le vecteur est nul!\n");
		exit(1);	
	}
	
	int max = vect[0];
	
	for(i=1; i<nb; i++){
		if(vect[i] > max)
			max = vect[i];
	}
	
	return max;
}

int main(){
	int vect[9]={5,2,6,1,9,12,3,25,1};
	
	int r = max_func(vect, 9);	
	printf("Valeur max: %d\n", r); 
	
	return 0;
}
