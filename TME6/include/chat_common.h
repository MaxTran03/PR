#ifndef H_CHAT_COMMON
#define H_CHAT_COMMON

#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

#include <sys/mman.h>
#include <sys/stat.h>        /* Pour les constantes des modes */
#include <fcntl.h>           /* Pour les constantes O_* */
#include <semaphore.h>
#include <signal.h>

#include <string.h>

#define MAX_MESS 50
#define MAX_USERS 10
#define TAILLE_MESS 1024

struct message {
  long type;
  char content[TAILLE_MESS];
};

struct myshm {
  int read;
  int write;
  int nb;
  sem_t sem;
  struct message messages[MAX_MESS];
};

char *getName(char *name);

#endif
