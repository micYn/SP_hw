#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#define BUF_SIZE 1024

void handleError(char *str){
	perror(str);
	exit(EXIT_FAILURE);
}
int main(int argc, char *argv[]){
	ssize_t numRead;
	char *buf = malloc(BUF_SIZE);
	int writeFd;
	int openFlags;
	mode_t filePerms;

	if(argc<2||argc>3||strcmp(argv[1],"--help")==0){
		fprintf(stderr,"%s [-a] file",argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if(strcmp(argv[1],"-a")==0){
		openFlags = O_CREAT|O_APPEND|O_WRONLY;
		filePerms = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;
		writeFd = open(argv[2],openFlags,filePerms);
		if(writeFd==-1){
			handleError("open");
		}
	}
	else{
		openFlags = O_CREAT|O_TRUNC|O_WRONLY;
		filePerms = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;
		writeFd = open(argv[1],openFlags,filePerms);
		if(writeFd==-1){
			handleError("open");
		}
	}

	while((numRead = read(STDIN_FILENO,buf,BUF_SIZE))>0){
		if(write(writeFd,buf,(size_t)numRead)==-1){
			perror("write");
		}
		if(write(STDOUT_FILENO,buf,(size_t)numRead)==-1){
			perror("write");
		}
		memset(buf,0,BUF_SIZE);
	}
	if(numRead==-1){
		handleError("read");
	}

	free(buf);

	if(close(writeFd)==-1){
		perror("close");
	}
	exit(EXIT_SUCCESS);
}