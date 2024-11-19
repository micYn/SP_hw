#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>

#include <errno.h>

#define BUF_SIZE 1024
bool all_zeros(char *buf, ssize_t numRead){
	for(ssize_t i=0;i<numRead;i++){
		if(buf[i]!=0){
			return false;
		}
	}
	return true;
}
void errorHandling(char *str){
	perror(str);
	exit(EXIT_FAILURE);
}
int main(int argc, char *argv[]){
	int srcFd, destFd;
	mode_t destfileMode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH; 
	ssize_t numRead;
	char buf[BUF_SIZE];

	if( argc!=3 || strcmp(argv[1] ,"--help")==0 ){
		fprintf(stderr,"%s sourceFile destinationFile", argv[0]);
		exit(EXIT_FAILURE);
	}
	if( (srcFd=open(argv[1], O_RDONLY)) == -1 ){
		errorHandling("open");
	}
	if( (destFd=open(argv[2], O_WRONLY|O_TRUNC|O_CREAT, destfileMode)) == -1){
		errorHandling("open");
	}
	while((numRead = read(srcFd, buf, BUF_SIZE))>0){
		if(all_zeros(buf, numRead)){
			if( lseek(destFd, (off_t)numRead, SEEK_CUR)==-1 ){
				errorHandling("lseek");
			}
		}
		else{
			if( write(destFd, buf, numRead)==-1 ){
				errorHandling("write");
			}
		}
		memset(buf, 0, BUF_SIZE);
	}
	if(numRead==-1){
		errorHandling("read");
	}
	exit(EXIT_SUCCESS);
}