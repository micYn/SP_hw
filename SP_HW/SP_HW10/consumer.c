#include "producer_consumer.h"

struct shmbuf *shm_buf;
int buffer_row;
pid_t producer_pid;

void handler(int sig, siginfo_t *info, void *ucontext){
	const int value = info->si_value.sival_int;
	char str[MSG_LENGTH];
	snprintf(str, MSG_LENGTH, "%d", value);
	// printf("%s\n%s\n", &shm_buf->buf[(value%buffer_row) * MSG_LENGTH], str);
	if(strcmp(&shm_buf->buf[(value%buffer_row) * MSG_LENGTH], str)==0){
		if(sem_wait(&shm_buf->sem1)==-1)	errorHandler("sem_wait");
		shm_buf->cnt++;
		if(sem_post(&shm_buf->sem1)==-1)	errorHandler("sem_post");
	}
	// else{
	// 	printf("consumer: data overwritten\n");
	// }
}

int main(int argc, char *argv[]){
	buffer_row = atoi(argv[1]);
	producer_pid = (pid_t)atoi(argv[2]);

	/* open the existing shared memory object */
	int shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0660);
	if(shm_fd==-1){
		perror("shm_open");
		exit(EXIT_FAILURE);
	}

	/* do a file shared mapping of it then close() */
	size_t total_size = sizeof(*shm_buf) + (buffer_row * MSG_LENGTH * sizeof(char));
	// struct shmbuf *shm_buf;
	shm_buf = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(shm_buf==MAP_FAILED){
		close(shm_fd);
		errorHandler("mmap");
	}
	if(close(shm_fd)==-1){		// no longer useful
		errorHandler("close");
	}

	/* prepare the signal handler */
	struct sigaction act, oldact;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	if(sigaction(SIGUSR1, &act, &oldact)==-1)	errorHandler("sigaction");


	/*******************************************************************************************/
	/* contiguously wait for signal and get the content within the shm_buf */
	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask, SIGUSR1);
	sigprocmask(SIG_BLOCK, &mask, NULL); // Unblock SIGUSR1
	printf("consumer: ready\n");
	fflush(stdout);
	for(;;){
		sigsuspend(&mask);
	}

}