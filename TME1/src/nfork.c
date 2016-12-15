/**** nfork.c ****/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int nfork(int nb_fils){
	int i, cpt=0;
	int pid;
	
	for(i=0; i<nb_fils; i++){
		if((pid = fork()) != 0){
			if(pid < 0){
				if(cpt == 0)
					return -1;
				return cpt;
			}else
				cpt++;
		}else
			return 0;
	}

	return cpt;
}



int main (int arg, char* argv []) {
    int p;
    int i=1; int N = 3;
    do {
        p = nfork (i);
        if (p != 0 )
               printf ("%d \n",p);
    } while ((p ==0) && (++i<=N));
    printf ("FIN \n");    
   return EXIT_SUCCESS;
}

