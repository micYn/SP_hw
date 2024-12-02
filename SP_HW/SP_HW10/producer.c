#include "producer_consumer.h"

int main(int argc, char *argv[]){
	if(argc!=5 || strcmp(argv[1], "--help")==0){
		fprintf(stderr, "Usage: %s [# of data] [transmission speed(ms)] [# of consumer] [buffer size]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	const int data_amount = atoi(argv[1]);
	const int transmission_speed = atoi(argv[2]);
	const int consumer_amount = atoi(argv[3]);
	const int buffer_row = atoi(argv[4]);
	if(data_amount<=0 || transmission_speed<=0 || consumer_amount<=0 || buffer_row<=0){
		fprintf(stderr, "Command line arguments need to be at least greater then 0\n");
		exit(EXIT_FAILURE);
	}

	/* create and truncate a POSIX shared memory object, memory mapping, close shm_fd*/
	int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT|O_EXCL|O_RDWR, 0660);
	if(shm_fd==-1){
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
	struct shmbuf *shm_buf;
	size_t total_size = sizeof(*shm_buf) + (buffer_row * MSG_LENGTH * sizeof(char));
	if(ftruncate(shm_fd, total_size)==-1){
		close(shm_fd);
		errorHandler("ftruncate");
	}
	shm_buf = mmap(NULL, total_size, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(shm_buf==MAP_FAILED){
		close(shm_fd);
		errorHandler("mmap");
	}
	if(close(shm_fd)==-1){		// no longer useful
		errorHandler("close");
	}

	if(sem_init(&shm_buf->sem1, 1, 1)==-1){
		errorHandler("sem_init");
	}

	if(sem_wait(&shm_buf->sem1)==-1)	errorHandler("sem_wait");
	shm_buf->cnt = 0;
	if(sem_post(&shm_buf->sem1)==-1)	errorHandler("sem_post");
	

	/* create consumer processes */
	pid_t producer_pid = getpid();
	unsigned int i, j;
	pid_t consumers[consumer_amount];
	for(i=0;i<consumer_amount;i++){
		pid_t pid = fork();
		switch(pid){
		case -1:
			errorHandler("fork");
		case 0:		//child's process
			char arg1[20], arg2[20];
			snprintf(arg1, sizeof(arg1),"%d", buffer_row);
			snprintf(arg2, sizeof(arg2),"%d", (int)producer_pid);
			execl("./consumer", "./consumer", arg1, arg2, (char *)NULL);
			errorHandler("execl");
		default:
			consumers[i] = pid;
			break;
		}
	}
	
	sleep(2); //so that every consumer is waiting
	
	/*******************************************************************************************/
	/* Starts periodically create new data*/
	for(i=0;i<data_amount;i++){
		printf("producer: sending %d\n", i);
		fflush(stdout);

		struct timespec start, end;
		long ms = 0;
		clock_gettime(CLOCK_REALTIME, &start);

		snprintf(&shm_buf->buf[(i%buffer_row) * MSG_LENGTH], MSG_LENGTH,  "%d", i);

		/* use sigqueue() to send signal to every consumer */
		union sigval value;
		value.sival_int = i;

		for(j=0;j<consumer_amount;j++){
			if(sigqueue(consumers[j], SIGUSR1, value)==-1)
				errorHandler("sigqueue");

			clock_gettime(CLOCK_REALTIME, &end);
			long sec = end.tv_sec - start.tv_sec;
			long nsec = end.tv_nsec - start.tv_nsec;

			if (nsec < 0) {
			    sec--;
			    nsec += 1e9;
			}

			ms = (sec * 1000) + (nsec / 1e6);

			if(ms > transmission_speed){
				fprintf(stderr, "timeout, sent until consumer %d\n", j);
				break;
			}
		}

		/* if there still time remain, sleep until then*/
		if(ms < transmission_speed){
			// printf("Time remaining\n");
			usleep((useconds_t)(transmission_speed - ms));
		}
	}

	/* kill the children */
	for(i=0;i<consumer_amount;i++){
		if(kill(consumers[i], SIGKILL)==-1)	errorHandler("kill");
	}

	if(sem_wait(&shm_buf->sem1)==-1)	errorHandler("sem_wait");
	printf("Total messages: %d\n", data_amount*consumer_amount);
	printf("Sum of received messages by all consumers:\n%d\n",shm_buf->cnt);
	printf("Loss rate: %.4lf%%\n", 100.0*(1-(double)shm_buf->cnt/(data_amount*consumer_amount)));
	if(sem_post(&shm_buf->sem1)==-1)	errorHandler("sem_post");

	/* munmap the mapping and unlink(remove) the shared memory. 
	Children's mapping won't be bothered as long as they already established the mapping */
	if(munmap(shm_buf, total_size)==-1)
		errorHandler("munmap");
	if(shm_unlink(SHARED_MEMORY_NAME)==-1)
		errorHandler("shm_unlink");

	exit(EXIT_SUCCESS);
}