#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
// #include <fcntl.h>

#include <errno.h>

int main(){
	int pipefd[2];
	if(pipe(pipefd)==-1){
		perror("pipe_ls pipe");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();
	switch(pid){
	case -1:
		perror("pipe_ls: fork");
		exit(EXIT_FAILURE);

	case 0:		// child exec ls but the outptu should go to pipe

		// now writing to stdout is actually writing to the pipe
		close(STDOUT_FILENO);
		if(dup(pipefd[1])!=STDOUT_FILENO){
			fprintf(stderr,"dup failed to return 1!\n");
 			exit(EXIT_FAILURE);
		}

		// we are writing, close unused read end
		close(pipefd[0]);

		// start writing the result of ls to the pipe:
		execlp("ls", "ls", "-la", (char*)NULL);

		// If execlp() succeeds, the child's process image is replaced by the image of program "ls"
		// and none of the code after execlp() will run. (the child will ends in the way that "ls" ends)
		// Thus if we reach this point, execlp() has failed so we handle the error without checking the return value.
		perror("pipe_ls: execlp");
		exit(EXIT_FAILURE);

	default:
		break;
	}

	/*Rest of parent's code:*/

	// Read from the pipe and write it through stdout
	close(pipefd[1]);
	ssize_t numRead;
	char buf[1024];

	while((numRead=read(pipefd[0], buf, sizeof(buf)))>0){
		if(write(STDOUT_FILENO,buf,numRead)==-1){
			perror("pipe_ls: write");
			exit(EXIT_FAILURE);
		}
	}
	if(numRead==-1){
		perror("pipe_ls: read");
		exit(EXIT_FAILURE);
	}

	if(waitpid(pid, (int*)NULL, 0)==-1){
		perror("pipe_ls: waitpid");
		exit(EXIT_FAILURE);
	}

	return 0;
}