#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <errno.h>

#define SHARED_MEMORY_NAME "/MY_SHARED_MEMORY"
#define MSG_LENGTH BUFSIZ

void errorHandler(char *msg){
	perror(msg);
	shm_unlink(SHARED_MEMORY_NAME);
	exit(EXIT_FAILURE);
}

struct shmbuf{
	sem_t sem1;	// avoid race condition between consumers modifying cnt
	int cnt; 	// accumulated by consumers (will be set to 0 by producer at first)
	char buf[];	// Note producer does not need to wait for semaphore for modifying buf[]
};

