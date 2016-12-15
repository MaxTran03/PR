/**** arbo.c ****/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    pid_t pid;
    int  j=0; int i = 0;
    while (i < 2) {
        i ++;
        if ((pid = fork ()) == -1) {
           perror ("fork");
           exit (1);
        }
        else if (pid == 0) j=i;
    }
    if (j == 2) {
        sleep (2);
        printf ("sans fils \n");
    }
    else {
      printf ("%d fils \n ", (i-j) );
      while (j < i) {
           j++;
           wait (NULL);
      }
    }
    return EXIT_SUCCESS;
}  
