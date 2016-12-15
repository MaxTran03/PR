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

extern int errno;

#define _POSIX_SOUCE 1
#define TAILLE_PATH 100

char buff_path [TAILLE_PATH];
DIR *pt_Dir;
struct dirent* dirEnt;


int main (int argc, char* argv []) {
	

	memcpy (buff_path,argv[2],strlen(argv[2]));
	
	if ( ( pt_Dir = opendir (buff_path) ) == NULL) {
		if (errno ==  ENOENT) {
			/* repertoire n'existe pas - créer le répertoire */
			if (mkdir ( buff_path, S_IRUSR|S_IWUSR|S_IXUSR) == -1) {
				perror ("erreur mkdir\n");
				exit (1);
			}
			else
				return 0;
		}
		else {
			perror ("erreur opendir \n");
			exit (1);
		}
	}
	
	int fd, n;
	char tab[1000];
	struct stat s;
	int cpt = 0;
	
	/* lire le contenu des fichiers du répertoire */
	while ((dirEnt= readdir (pt_Dir)) !=NULL) {
		char az[1024]="";
		strcpy(az, argv[2]);
		strcat(az, "/");
		strcat(az, dirEnt->d_name);
		
		if(!stat(az, &s)){
			if(S_ISREG(s.st_mode)){
			 
				if ((fd = open (az, O_RDWR, 0600)) == -1)
				  return EXIT_FAILURE;
			  
				strcpy(tab,"");
				if((n = read(fd, &tab, 1000*sizeof(char)))==-1)
					return EXIT_FAILURE;
			
				if(strstr(tab,argv[1]) != NULL){
					cpt ++;
					printf("Le mot '%s' est trouvé dans le fichier %s\n",argv[1], dirEnt->d_name);
				}
			}
		}
	}
	
	if(cpt == 0)
		printf("Aucun fichier valide\n");
	
	closedir (pt_Dir);
	close(fd);
	
	return 0;
}




