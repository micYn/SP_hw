/*
./client without ./server & first

*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>


#define FIFO_PATH "/tmp/my_fifo"

int main(int argc, char *argv[]) {
	if(argc==1){
		// we open the write end but do not attempt to write:
    	int fifo_fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);	// Open FIFO in nonblocking mode for writing (if the server haven't open the read end, this will fail)
	    if(fifo_fd==-1){
	        perror("open FIFO for writing");
	        if(errno==ENXIO){
	            printf("No reader is available on the FIFO.\n");
	        }
	        exit(EXIT_FAILURE);
	    }
	    close(fifo_fd);
	}
    else if(atoi(argv[1])==1){
    	//we open write end but we don't write anything to the FIFO, 
    	//so when the read end attempts to read, there will be an specific error
    	//Open FIFO in nonblocking mode for writing (if the server haven't open the read end, this will fail)
	    int fifo_fd = open(FIFO_PATH, O_WRONLY);	// Open FIFO in nonblocking mode for writing (if the server haven't open the read end, this will fail)
	    if(fifo_fd==-1){
	        perror("open FIFO for writing");
	        if(errno==ENXIO){
	            printf("No reader is available on the FIFO.\n");
	        }
	        exit(EXIT_FAILURE);
	    }
	    int flags;
	    flags = fcntl(fifo_fd, F_GETFL);
	    flags |= O_NONBLOCK;
	    fcntl(fifo_fd, F_SETFL, flags);

	    close(fifo_fd);
    }
    else if(atoi(argv[1])==2){
    	if(signal(SIGPIPE, SIG_IGN)==SIG_ERR){
    		perror("signal");
			exit(EXIT_FAILURE);
    	}
    	// Attempt to write to FIFO in nonblocking mode
    	int fifo_fd = open(FIFO_PATH, O_WRONLY);	// Open FIFO in nonblocking mode for writing (if the server haven't open the read end, this will fail)
	    if(fifo_fd==-1){
	        perror("open FIFO for writing");
	        if(errno==ENXIO){
	            printf("No reader is available on the FIFO.\n");
	        }
	        exit(EXIT_FAILURE);
	    }
	    int flags;
	    flags = fcntl(fifo_fd, F_GETFL);
	    flags |= O_NONBLOCK;
	    fcntl(fifo_fd, F_SETFL, flags);
	    sleep(1);
	    const char *message = "Hello from server!";
	    ssize_t bytes_written = write(fifo_fd, message, strlen(message));

	    if (bytes_written == -1) {
	        perror("write to FIFO");
	        if (errno == EPIPE) {
	            printf("No reader open on the FIFO, or reader has closed.\n");
	        }
	    } else {
	        printf("Client wrote %ld bytes to the FIFO.\n", bytes_written);
	    }
	    close(fifo_fd);
    }
    else{

    }
   	
    return 0;
}
