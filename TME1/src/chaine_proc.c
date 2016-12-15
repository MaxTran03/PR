#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(){
	
	int i;
	pid_t pid;
	pid_t tab[10];
	int status;
	
	srand(time(NULL));
	
	for(i=0;i<10;i++){
		
		if ((pid=fork())!=0){
			waitpid(pid,&status,WUNTRACED);
			printf("mon papa %d, moi %d, mon fils %d\n",getppid(),getpid(),pid);
			if (i==0)
				printf("Valeur rand: %d\n",WEXITSTATUS(status));
			exit(WEXITSTATUS(status));
		}else{
			tab[i] = getppid();
			if(i==9){
				int j;
				for(j=0;j<10;j++){
					printf("pid %d : %d \n", j, tab[j]);
				}
				int val = (int)(rand () /(((double) RAND_MAX +1) /100));
				printf("Le dernier fils a choisi: %d\n",val);
				exit(val);
			}
				
		}
		
	}
	
	
	
	return 0;
	
}
