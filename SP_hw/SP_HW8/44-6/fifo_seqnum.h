#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define SERVER_FIFO "/tmp/seqnum_sv"	//A well-known name in order for clients to know where to send requests


#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld"	//A consensus template of client FIFO name so the server will know which FIFO it has to send response to
													//%ld would be the client's pid which will be specified in the request msg from the client

#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE)+20)	//Space for client FIFO pathname (+20 as a generous allowance for the pid)

struct request{		// A fixed length request format (client--->server)
	pid_t pid;
	int seqLen;
};

struct response{	// Server respoense format (server--->client)
	int seqNum;
};
