#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void hand(int sig){}

int main (void) {
	sigset_t sig_proc;
	struct sigaction action;
	
	sigfillset(&sig_proc);
	sigdelset(&sig_proc, SIGUSR1);
	
	action.sa_flags = 0;
	action.sa_mask = sig_proc;
	action.sa_handler = hand;
	sigaction(SIGUSR1, &action, NULL);
	sigprocmask(SIG_SETMASK, &sig_proc, NULL);


    int fd1, fd2, fd3;
    if ((fd1 = open ("./fich1", O_RDWR| O_CREAT | O_TRUNC, 0600)) == -1)
        return EXIT_FAILURE;
    if (write (fd1,"abcde", strlen ("abcde")) == -1)
        return EXIT_FAILURE;
    if (fork () == 0) {
        if ((fd2 = open ("./fich1", O_RDWR)) == -1)
            return EXIT_FAILURE;
        if (write (fd1,"123", strlen ("123")) == -1)
            return EXIT_FAILURE;
        sleep(1);
        kill(getppid(),SIGUSR1);
        if (write (fd2,"45", strlen ("45")) == -1)
                return EXIT_FAILURE;
        close(fd2); 
    } else {
        fd3 = dup(fd1);
        sigsuspend(&sig_proc);
        printf("allo\n");
        if (lseek (fd3,0,SEEK_SET) == -1)
            return EXIT_FAILURE;
        if (write (fd3,"fg", strlen ("fg")) == -1)
            return EXIT_FAILURE;
        if (write (fd1,"hi", strlen ("hi")) == -1)
            return EXIT_FAILURE;
        wait (NULL);
        close(fd1);
        close(fd3);
        return EXIT_SUCCESS;
    }
   
    
    return 0;
}
