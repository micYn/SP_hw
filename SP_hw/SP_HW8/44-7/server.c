#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/my_fifo"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	// Server should run first and creates a FIFO:
    if(mkfifo(FIFO_PATH, S_IRUSR|S_IWUSR|S_IWGRP)==-1 && errno!=EEXIST){
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    if(argc==1){
    	// we don't open read end in this case
    }
    else if(atoi(argv[1])==1){
    	// Open FIFO in nonblocking mode for reading (this will always succeed in contrast of opening in nonblocking mode for writing)
	    int fifo_fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
	    if(fifo_fd==-1){
	        perror("open FIFO for reading");
	        exit(EXIT_FAILURE);
	    }
	    else{
	    	printf("read end opened successfully\n");
	    }
	    close(fifo_fd);
    }
    else if(atoi(argv[1])==2){
	    int fifo_fd = open(FIFO_PATH, O_RDONLY);
	    if(fifo_fd==-1){
	        perror("open FIFO for reading");
	        exit(EXIT_FAILURE);
	    }
	    int flags;
	    flags = fcntl(fifo_fd, F_GETFL);
	    flags |= O_NONBLOCK;
	    fcntl(fifo_fd, F_SETFL, flags);

	    // Attempt to read from FIFO in nonblocking mode
	    char buffer[BUFFER_SIZE];
	    ssize_t bytes_read = read(fifo_fd, buffer, BUFFER_SIZE - 1);

	    if (bytes_read==-1) {
	        if(errno==EAGAIN){
	            printf("No writer on the other end.\n");
	            exit(EXIT_FAILURE);
	        }
	        else{
	            perror("read from FIFO");
	            exit(EXIT_FAILURE);
	        }
	    }
	    else if(bytes_read==0){
	        printf("No data available to read (FIFO empty).\n");
	        exit(EXIT_FAILURE);
	    }
	    else{
	        buffer[bytes_read] = '\0';
	        printf("Client read %ld bytes from the FIFO: %s\n", bytes_read, buffer);
	    }
	    close(fifo_fd);
    }
    else if(atoi(argv[1])==3){
	    int fifo_fd = open(FIFO_PATH, O_RDONLY|O_NONBLOCK);
	    if(fifo_fd==-1){
	        perror("open FIFO for reading");
	        exit(EXIT_FAILURE);
		}

	    // Attempt to read from FIFO in nonblocking mode
	    char buffer[BUFFER_SIZE];
	    ssize_t bytes_read = read(fifo_fd, buffer, BUFFER_SIZE - 1);

	    if (bytes_read==-1) {
	        if(errno==EAGAIN){
	        	printf("No writer on the other end.\n");
	        	exit(EXIT_FAILURE);
	        }
	        else{
	            perror("read from FIFO");
	        	exit(EXIT_FAILURE);
	        }
	    }
	    else if(bytes_read==0){
	            printf("No data available to read (FIFO empty).\n");
	        	exit(EXIT_FAILURE);
	    }
	    else{
	        buffer[bytes_read] = '\0';
	        printf("Client read %ld bytes from the FIFO: %s\n", bytes_read, buffer);
	    }
	    close(fifo_fd);
    }
    else if(atoi(argv[1])==4){
	    int fifo_fd = open(FIFO_PATH, O_RDONLY);
	    if(fifo_fd==-1){
	        perror("open FIFO for reading");
	        exit(EXIT_FAILURE);
		}
	    close(fifo_fd);
    }
    
    
    return 0;
}
