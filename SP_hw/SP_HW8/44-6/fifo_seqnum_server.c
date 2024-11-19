#include <signal.h>
#include <time.h>
#include "fifo_seqnum.h"

#define MAX_RETRIES 5
#define RETRY_INTERVAL 1

int
main(int argc, char *argv[])
{
	int serverFd, dummyFd;
	int clientFd;
	struct request req;
	struct response resp;
	char clientFifo[CLIENT_FIFO_NAME_LEN];
	int seqNum = 0;

	/*Create a server FIFO which every client knows its name*/
	umask(0);
	if(mkfifo(SERVER_FIFO, S_IRUSR|S_IWUSR|S_IWGRP)==-1 && errno != EEXIST)
		errExit("mkfifo %s", SERVER_FIFO);

	serverFd = open(SERVER_FIFO, O_RDONLY);
	if(serverFd==-1)
		errExit("open %s", SERVER_FIFO);

	/*Server opens a write descriptor itself, prevent the server from getting EOF if all the clients close the write end of the FIFO*/
	dummyFd = open(SERVER_FIFO, O_WRONLY);
	if(dummyFd==-1)
		errExit("open %s", SERVER_FIFO);

	/* If this server writes to a client FIFO that does not have the read end opened, 
	   the server was supposed to be delivered a SIGPIPE(get killed) and errno=EPIPE. Here we don't let the first thing happen.

	   Well, we might argue that this is redundant cuz write() will never be executed if the read end is not opened by the client
	   because this server will be blocked here. But we have to consider the situation 
	   when client closes the read end during or before this server writes to FIFO, which causes SIGPIPE as well.
	*/
	if(signal(SIGPIPE, SIG_IGN)==SIG_ERR)
		errExit("signal");

	/**************************************************************************/
	for(;;)
	{
		if(read(serverFd, &req, sizeof(struct request))!=sizeof(struct request))
		{
			fprintf(stderr, "Error reading request; discarding\n");
			continue;
		}
		snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long)req.pid);

		// clientFd = open(clientFifo, O_WRONLY); ---> if the client doesn't open the read end of this FIFO, server will stuck here!
		// So we revise it like this:
		int retries = 0;
		while((clientFd = open(clientFifo, O_WRONLY|O_NONBLOCK))==-1 && errno==ENXIO && retries<MAX_RETRIES)
		{
			// O_NONBLOCK will make open either succeeds immediately or fails immediately and returns ENXIO.
			retries++;
			sleep(RETRY_INTERVAL);	//sleep for a few seconds before another try
		}
		if(clientFd==-1)
		{
			errMsg("open %s", clientFifo);
			continue;
		}
		else	// reset blocking mode so subsequent read() write() will operate in blocking mode
		{
			int flags = fcntl(clientFd, F_GETFL, 0);
	        if (flags == -1 || fcntl(clientFd, F_SETFL, flags & ~O_NONBLOCK) == -1) {
	            perror("fcntl");
	            close(clientFd);
	            exit(EXIT_FAILURE);
	        }
		}

		/* start sending response and close the client FIFO */
		resp.seqNum = seqNum;
		if(write(clientFd, &resp, sizeof(struct response))!= sizeof(struct response))
			fprintf(stderr, "Error writing to FIFO %s\n", clientFifo);
		if(close(clientFd)==-1)
			errMsg("close");

		seqNum += req.seqLen;
	}
}