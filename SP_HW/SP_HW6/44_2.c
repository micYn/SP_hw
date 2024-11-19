#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct{
	FILE *stream;
	pid_t pid;
} popen_t;

popen_t *my_popen(const char *command, const char *type){
	popen_t *popen_t_inst = malloc(sizeof(popen_t));
	if(popen_t_inst == NULL){
		perror("malloc");
		return NULL;
	}
	
	int mypipe[2];
	if(pipe(mypipe)==-1){
		perror("pipe");
		return NULL;
	}

	pid_t pid;
	switch(pid=fork()){
	case -1:
		perror("fork");
		return NULL;
	case 0:		//child
		if(strcmp(type,"r")==0){	//child has to send its output to parent. it is used to send output to stdout
			if(close(mypipe[0])==-1){
				perror("close");
				return NULL;
			}
			if(mypipe[1]!=STDOUT_FILENO){
				dup2(mypipe[1], STDOUT_FILENO);
				close(mypipe[1]);
			}
			// Now this child still think it is sendin output to stdout but it is actually going to the pipe.
			execlp("/usr/bin/sh","sh","-c",command, (char*)NULL);
			perror("execlp");
			exit(EXIT_FAILURE);
		}
		else if(strcmp(type,"w")==0){	//child has to get its input from parent, though it is used to get input from stdin
			if(close(mypipe[1])==-1){
				perror("close");
				return NULL;
			}
			if(mypipe[0]!=STDIN_FILENO){
				dup2(mypipe[0], STDIN_FILENO);
				close(mypipe[0]);
			}
			execlp("/usr/bin/sh","sh","-c",command, (char*)NULL);
			perror("execlp");
			exit(EXIT_FAILURE);
		}
		else{
			fprintf(stderr, "The 2nd argument can only be \"r\" or \"w\".\n");
			return NULL;
		}

	default:	//parent
		break;
	}
	// rest of parent's code:
	popen_t_inst->pid = pid;
	if(strcmp(type, "r")==0){
		if(close(mypipe[1])==-1){
			perror("close");
			return NULL;
		}
		popen_t_inst->stream = fdopen(mypipe[0],"r");
		if(popen_t_inst->stream==NULL){
			perror("fdopen");
			return NULL;
		}
	}
	else if(strcmp(type, "w")==0){
		if(close(mypipe[0])==-1){
			perror("close");
			return NULL;
		}
		popen_t_inst->stream = fdopen(mypipe[1],"w");
		if(popen_t_inst->stream==NULL){
			free(popen_t_inst);
			perror("fdopen");
			return NULL;
		}
	}
	else{
		fprintf(stderr, "The 2nd argument can only be \"r\" or \"w\".\n");
		return NULL;
	}

	return popen_t_inst;
}
int my_pclose(popen_t *popen_t_inst){
	if(fclose(popen_t_inst->stream)==EOF){
		perror("fclose");
		return -1;
	}
	int status;
	if(waitpid(popen_t_inst->pid,&status,1)==-1){
		perror("waitpid");
		return -1;
	}
	free(popen_t_inst);

	return status;
}
int main(){
	popen_t *popen_t_inst = my_popen("ls -l", "r");
	// popen_t *popen_t_inst = my_popen("cat", "w");
	if(popen_t_inst==NULL){
		fprintf(stderr,"my_popen() failed for some reason\n");
		exit(EXIT_FAILURE);
	}
	char buffer[256];
    while (fgets(buffer, sizeof(buffer), popen_t_inst->stream) != NULL) {
        fputs(buffer, stdout);
    }

    // fprintf(popen_t_inst->stream, "Hello from my_popen!\n");
    // fprintf(popen_t_inst->stream, "Writing another line.\n");
	// fflush(popen_t_inst->stream);

    int status = my_pclose(popen_t_inst);
    if(status==-1){
    	fprintf(stderr,"my_pclose() failed for some reason\n");
    	exit(EXIT_FAILURE);
    }
    printf("Command exited with status %d\n", WEXITSTATUS(status));

    exit(EXIT_SUCCESS);
}