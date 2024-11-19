#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#define BUF_SIZE 128
void errorHandling(char *str){
	perror(str);
	exit(EXIT_FAILURE);
}
extern char **environ;
int main(){
	ssize_t numRead;
	char buf[BUF_SIZE];
	char theCmd[BUF_SIZE];
	char** cmdToken;

	while(1){
		int pd[2];
		if(pipe(pd)==-1){
			errorHandling("pipe");
		}
		switch(fork()){
		case -1:
			errorHandling("fork");
		case 0:		
		//child, read the cmd from pipe, 
		//executes the command and send it to the commander
			if(close(pd[1])==-1){
				errorHandling("close");
			}
			numRead = read(pd[0], buf, BUF_SIZE);
			if(numRead == -1){
				errorHandling("read in child");
			}
			buf[strlen(buf)-1] = '\0';
			// if(write(1,buf, numRead)==-1){
			// 	errorHandling("write in child");
			// }
			
			cmdToken = (char**)malloc(10*sizeof(char*));
			if(cmdToken==NULL){
				errorHandling("malloc");
			}
			char *tmpstr;
			tmpstr = strtok(buf, " ");
			int i = 0;
			while(tmpstr!=NULL){
				cmdToken[i] = (char*)malloc(20*sizeof(char));
				if(cmdToken==NULL){
					errorHandling("malloc");
				}
				strcpy(cmdToken[i], tmpstr);
				// printf("%d %s\n", i, cmdToken[i]);
				i++;
				tmpstr = strtok(NULL, " ");
			}
			cmdToken[i] = (char*)NULL;

			execvpe(cmdToken[0], cmdToken, environ);

			errorHandling("execvp");
		default:
			break;
		}
		printf("Commander, please input command: ");
		fflush(stdout);
		fgets(theCmd, BUF_SIZE, stdin);
		// printf("%s", theCmd);

		if(close(pd[0])==-1){
			errorHandling("close");
		}
		if(write(pd[1], theCmd, strlen(theCmd))==-1){
			errorHandling("write");
		}
		wait(NULL);
	}
	

	exit(EXIT_SUCCESS);
}