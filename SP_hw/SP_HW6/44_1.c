#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void errorHandling(char *str){
	perror(str);
	exit(EXIT_FAILURE);
}
int main(int argc, char *argv[]){
	if(argc!=1 || argc==2&&strcmp(argv[1],"--help")==0){
		fprintf(stderr, "Usage: %s\n", argv[0]);
	}

	ssize_t numRead;
	char buf[1024];

	int parent_to_child[2];
	if(pipe(parent_to_child)==-1){
		errorHandling("pipe");
	}
	int child_to_parent[2];
	if(pipe(child_to_parent)==-1){
		errorHandling("pipe");
	}

	pid_t pid;
	switch(pid=fork()){
		case -1:	// fork() failed
			errorHandling("fork");
		
		case 0:		// child process
			if(close(parent_to_child[1])==-1){
				errorHandling("close");
			}
			if(close(child_to_parent[0])==-1){
				errorHandling("close");
			}
			
			for(;;){
				numRead = read(parent_to_child[0], buf, sizeof(buf));
				if(numRead==-1){
					errorHandling("read");
				}
				if(strcmp(buf, "kill yourself\n")==0){
					exit(EXIT_SUCCESS);
				}
				int i;
				for(i=0;i<numRead;i++){
					if(buf[i]<=122 && buf[i]>=97){
						buf[i] -= 32;
					}
				}
				if(write(child_to_parent[1], buf, numRead)==-1){
					errorHandling("write");
				}
			}
		default:
			if(close(parent_to_child[0])==-1){
				errorHandling("close");
			}
			if(close(child_to_parent[1])==-1){
				errorHandling("close");
			}
			const char *prompt = "Enter some text(type \"kill yourself\" to exit): ";
			for(;;){
				if(write(STDOUT_FILENO, prompt, strlen(prompt))==-1){
					errorHandling("write");
				}
				numRead = read(STDIN_FILENO, buf, sizeof(buf));
				if(numRead==-1){
					errorHandling("read");
				}

				if(write(parent_to_child[1], buf, numRead)==-1){
					errorHandling("write");
				}
				if(strcmp(buf, "kill yourself\n")==0){
					exit(EXIT_SUCCESS);
				}

				numRead = read(child_to_parent[0], buf, sizeof(buf));
				if(numRead==-1){
					errorHandling("read");
				}
				if(write(STDOUT_FILENO, buf, numRead)==-1){
					errorHandling("write");
				}
			}
	}
	

}