/*
 * redirect_out.c   :   check for >
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"
#define STD_OUTPUT 1
#define STD_INPUT  0

/*
 * Look for ">" in myArgv, then redirect output to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_out(char ** myArgv) {
	int i = 0;
  	int fd;

  	/* search forward for >
  	 * Fill in code. */
  	int find = 0;
  	while(myArgv[i]!=NULL){
  		if(strcmp(myArgv[i],">")==0){
  			find = 1;
  			break;
  		}
  		i++;
  	}
  	if(find==0)	return -1;

  	if (myArgv[i]) {	/* found ">" in vector. */

    	/* 1) Open file.
    	 * 2) Redirect to use it for output.
    	 * 3) Cleanup / close unneeded file descriptors.
    	 * 4) Remove the ">" and the filename from myArgv.
		 *
    	 * Fill in code. */

  		// mode_t modes = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;
  		fd = open(myArgv[i+1],O_WRONLY|O_CREAT|O_TRUNC, 0660);
  		if(fd==-1){
  			perror("open");
  			exit(EXIT_FAILURE);
  		}
  		if(dup2(fd, STDOUT_FILENO)==-1){
  			perror("dup2");
  			exit(EXIT_FAILURE);
  		}

  		close(fd);
  		
  		myArgv[i+1] = (char*)NULL;
  		myArgv[i] = (char*)NULL;
  	}
  	return 0;
}
