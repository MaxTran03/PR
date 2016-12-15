#define _SVID_SOURCE 1
#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/shm.h>
#include <setjmp.h>

#define MAX_SIZE 10
#define SEM_PRODS 0
#define SEM_CONS 1
#define MUTEX 2

int NBPRODS, NBCONS;

/* SHM */
key_t key;
char *adr_att;

typedef struct {
	int size;
	char content[MAX_SIZE];
} shared;
shared *stack;


/* SEM */
key_t semkey;
int semid;
struct sembuf op[3];

int fils;
struct sigaction sigact;
static jmp_buf env;

void sigint_handler(int sig){
	if(fils == 1)
		exit(EXIT_SUCCESS);
}

void P(int sem){
	op[sem].sem_op = -1;	
	semop(semid, &op[sem], 1);
}

void V(int sem){
	op[sem].sem_op = 1;	
	semop(semid, &op[sem], 1);
}

void push(char c){
	/* Operation P sur SEM_PRODS (on bloque si sem = 0) */
	/* Verifier que la pile ne soit pas pleine */
	P(SEM_PRODS);	
	
	P(MUTEX);	
	stack->content[stack->size] = c;
	stack->size++;
	V(MUTEX);
	
	printf("PUSH %c, stack size : %d, pid : %d\n", stack->content[stack->size-1],stack->size, getpid());

	/* Operation V sur SEM_CONS */
	/* Informer aux consomateurs qu'ils peuvent consommer */
	V(SEM_CONS);
}

char pop(){
	char res;

	/* Operation P sur SEM_CONS  (on bloque si sem = 0) */
	/* Verifier que la pile ne soit pas vide */
	P(SEM_CONS);
	
	P(MUTEX);	
	res = stack->content[stack->size-1];
	stack->size--;
	printf("POP %c, stack size : %d, pid : %d\n", res, stack->size, getpid());
	V(MUTEX);
	
	/* Operation V sur SEM_PRODS */
	/* Informer aux producteurs qu'ils peuvent produire */
	V(SEM_PRODS);
	return res;	
}

void producteur() {
	char c;   
   while((c = getchar()) != EOF){ 	
   	push(c);     
   }
   exit(0);
}

void consommmateur() {
   while(1){  	
		putchar(pop());     
      fflush(stdout);
   } 
   exit(0);
}



int main(int argc, char ** argv){
   int i;
	
	printf("Début MAIN\n");
	
	/* Termine le prog proprement en detruisant les sem par ctrl-c */
	fils = 0;
	sigact.sa_handler = sigint_handler;
   sigaction(SIGINT, &sigact, NULL);
    
   if(argc != 3){
   	perror("Error argument!\n");
   	return EXIT_FAILURE;
   }
   
   NBPRODS = atoi(argv[1]);
   NBCONS = atoi(argv[2]);
    
   /* Creation et initialisation des sems */
	semkey = ftok(argv[0], 'Q');
	semid = semget(semkey, 3, IPC_CREAT|IPC_EXCL|0666);
	semctl(semid, SEM_PRODS, SETVAL, MAX_SIZE);
	semctl(semid, SEM_CONS, SETVAL, 0);
	semctl(semid, MUTEX, SETVAL, 1);
	op[0].sem_num = 0;
	op[0].sem_flg = 0;
	op[1].sem_num = 1;
	op[1].sem_flg = 0;
	op[2].sem_num = 2;
	op[2].sem_flg = 0;
	
	/* Creation ou recup du shm */
	key = ftok(argv[0], 'M');
	int shm_id = shmget(key, sizeof(shared), 0666 | IPC_CREAT);
	adr_att = shmat(shm_id, 0, 0600);
	
	/* Association de shm struct */
	stack = (shared*) adr_att;
	stack->size = 0;
   
   /* Creations des producteurs */
	for(i=0; i<NBPRODS; i++){
		if(fork() == 0){
			fils = 1;
    		producteur(i); 
		}    	
	}
    
	/* Creation des consommateurs */
	for(i=0; i<NBCONS; i++){
		if(fork() == 0){
			fils = 1;
		 	consommmateur(i); 
		 }
	}
	
	for(i=0; i<(NBPRODS+NBCONS); i++){
		wait(NULL);
	}
	
	shmdt(adr_att);
	
	/* Suppression de shm */
	shmctl(shm_id, IPC_RMID, 0);
	
	printf("\nDestruction des semaphores...\n");
	semctl(semid, 0, IPC_RMID, 0);
	
	printf("\nFin MAIN\n");
	
	return EXIT_SUCCESS;	

}
