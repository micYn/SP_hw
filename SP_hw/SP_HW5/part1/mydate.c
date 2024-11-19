#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

void parse_and_print(char *time_str){

	char* token[7];
	//token[0~6] : dayname, month, day, hr, min, sec, year

	char delimiters[] = " :";
	token[0] = strtok(time_str, delimiters);
	int cnt = 0;
	while(token[cnt]!=NULL){
		// printf("%s\n", token[cnt]);
		token[++cnt] = strtok(NULL, delimiters);
	}

	char *PM_AM;
	int hr = atoi(token[3]);
	if(hr>=12){
		PM_AM = "PM";
		hr -= 12;
	}else{
		PM_AM = "AM";
	}

	printf("%s %s(%s), %s %d:%s %s\n", token[1], token[2], token[0], token[6], hr, token[4],PM_AM);

}

int main(){
	time_t tm;
	if(time(&tm)==-1){
		perror("mydate: time");
		exit(EXIT_FAILURE);
	}
	char *time_str = ctime(&tm);
	if(time_str == NULL){
		perror("mydate: ctime");
		exit(EXIT_FAILURE);
	}

	time_str[strlen(time_str)-1] = '\0';
	parse_and_print(time_str);

	return 0;
}