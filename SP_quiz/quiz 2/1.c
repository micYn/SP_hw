#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
		do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define DATA_UPPERBOUND 9

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_variable = PTHREAD_COND_INITIALIZER;

struct thread_info{
	pthread_t thread_id;
	int thread_num;		//1 for producer cuz there will be only one producer
};

char termination_char;
char buffer = '&';

void * consumer_routine(void *arg){
	int s;
	int *sum;
	sum = (int *)malloc(sizeof(int));
	*sum = 0;
	for(;;){
		s = pthread_mutex_lock(&mtx);
		if(s!=0)
			handle_error_en(s, "pthread_mutex_lock");

		// while(buffer=='&'){
			s = pthread_cond_wait(&cond_variable, &mtx);
			if(s!=0)
				handle_error_en(s, "pthread_cond_wait");
		// }
		
		if(buffer == termination_char)
			break;
		*sum += buffer - '0';
		// printf("%lu: %d\n", pthread_self(), *sum);

		s = pthread_mutex_unlock(&mtx);
		if(s!=0)
			handle_error_en(s, "pthread_mutex_unlock");
	}
	printf("%lu, terminating\n", pthread_self());
	pthread_exit((void *)sum);
}
void * producer_routine(void *arg){
	srand(time(NULL));
	int i,s;
	void *res;
	int *retval;
	struct producer_info{
		double time_interval;
		int num_datapoints;
		int num_consumers;
	};
	struct producer_info p_info = *(struct producer_info *)arg;
	double time_interval = p_info.time_interval;
	int num_datapoints = p_info.num_datapoints;
	int num_consumers = p_info.num_consumers;

	struct thread_info *consumer;
	consumer = calloc(num_consumers, sizeof(*consumer));

	for(i=0;i<num_consumers;i++){
		consumer[i].thread_num = i+1;
		s = pthread_create(&consumer[i].thread_id, NULL, &consumer_routine, NULL);
		if(s!=0)
			handle_error_en(s, "pthread_create");
		printf("create %d\n", consumer[i].thread_num);
	}
	termination_char = '#';

	for(i=1;i<=num_datapoints;i++){

		buffer = (char)(rand()%(DATA_UPPERBOUND+1) + '0');

		s = pthread_cond_broadcast(&cond_variable);
		if(s!=0)
			handle_error_en(s, "pthread_cond_broadcast");

		sleep(time_interval);
	}

	buffer = termination_char;

	/***************************************/

	for(i=0;i<num_consumers;i++){
		s = pthread_join(consumer[i].thread_id, &res);
		if(s!=0)
			handle_error_en(s, "pthread_join: consumer");
		printf("Get a return value from consumer %lu : %d\n", consumer[i].thread_id,*(int *)res);
		fflush(stdout);
	}

	free(res);
	pthread_exit((void *)NULL);
}

int main(int argc, char *argv[]){
	if(argc!=4 || strcmp(argv[1], "--help")==0){
		fprintf(stderr, "Usage: %s <data time interval> <# of consumer> <# of data point>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	struct producer_info{
		double time_interval;
		int num_datapoints;
		int num_consumers;
	};
	struct producer_info p_info;
	p_info.time_interval = atof(argv[1]);
	p_info.num_datapoints = atoi(argv[3]);
	p_info.num_consumers = atoi(argv[2]);

	int s;
	void *res;	// for thread return value

	struct thread_info producer;
	producer.thread_num = 1;
	s = pthread_create(&producer.thread_id, NULL, &producer_routine, &p_info);
	if(s!=0)
		handle_error_en(s, "pthread_create");

	s = pthread_join(producer.thread_id, &res);
	if(s!=0)
		handle_error_en(s, "pthread_join: producer");

	if(res!=NULL)	free(res);
	exit(EXIT_SUCCESS);
}