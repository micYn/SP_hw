#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <fcntl.h>

#include <errno.h>

int main(int argc, char *argv[]){
	if(argc!=2){
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		perror("mycat: open");
		exit(EXIT_FAILURE);
	}

	ssize_t numRead;
	char buf[1024];
	while( (numRead=read(fd,buf,sizeof(buf)))>0 ){
		if(write(1,buf,numRead)==-1){
			perror("mycat: write");
			exit(EXIT_FAILURE);
		}
		memset(buf, 0, sizeof(buf));
	}
	if(numRead==-1){	//namely while loop ends due to an error of read()
		perror("mycat: read");
			exit(EXIT_FAILURE);
	}

	return 0;
}