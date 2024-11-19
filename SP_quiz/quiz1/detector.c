#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
void errorHandling(char *str){
	perror(str);
	exit(EXIT_FAILURE);
}
int cnt = 0;
void alarmHandler(int sig){
	if(cnt!=0){
		//mail:
		printf("Error, sending a mail(這信要自己打內容我忘記怎麼自動填內容了):\n");
		system("mailx -s Error deac");
		// system("ls -l");
		cnt=0;
	}
	else{
		// printf("you are good\n");
	}
}
void intHandler(int sig){
	cnt++;
}
int main(int argc, char *argv[]){
	if(argc!=3 || strcmp(argv[1], "--help")==0){
		fprintf(stderr, "Usage: %s start_time cycle_time\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	sigset_t tmpmask;	//tmpmask for sigsuspend
	if(sigfillset(&tmpmask)==-1){
		errorHandling("sigfillset");
	}
	if(sigdelset(&tmpmask, SIGINT)==-1){
		errorHandling("sigdelset");
	}
	if(sigdelset(&tmpmask, SIGALRM)==-1){
		errorHandling("sigdelset");
	}

	(void)sigset(SIGALRM, alarmHandler);
	(void)sigset(SIGINT, intHandler);

	struct itimerval val;
	val.it_interval.tv_sec = atoi(argv[2]);
	val.it_interval.tv_usec = 0;
	val.it_value.tv_sec = atoi(argv[1]);
	val.it_value.tv_usec = 0;
	if(setitimer(ITIMER_REAL, &val, NULL)==-1){
		errorHandling("setitimer");
	}
	printf("checking forever\n");
	fflush(stdout);
	for(;;){
		sigsuspend(&tmpmask);
	}
}