#include "posixmq_file.h"

void cleanup(int sig)
{
	if(mq_unlink(SERVERMQ_NAME)==-1)
	{
		perror("mq_unlink");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
static void
grimReaper(int sig)
{
	int savedErrno = errno;
	while(waitpid(-1, NULL, WNOHANG)>0)
	{
		continue;
	}
	errno = savedErrno;
}
void serveRequest(const struct req *request)
{
	mqd_t client_mqd = mq_open(request->clientmq_name, O_WRONLY);
	if(client_mqd==(mqd_t)-1)
	{
		perror("mq_open");
		exit(EXIT_FAILURE);
	}
	struct res response;
	int fd = open(request->filename, O_RDONLY);

	// open() failed:
	if(fd==-1)
	{
		response.type = RESP_TYPE_FAILURE;
		snprintf(response.content, sizeof(response.content), "%s", "Couldn't open the file\n");
		mq_send(client_mqd, (char *)&response, sizeof(response), 0);
		exit(EXIT_FAILURE);
	}
	
	// file opened, transmit the file:
	ssize_t numRead;
	response.type = RESP_TYPE_DATA;
	memset(response.content, '\0', sizeof(response.content));
	while((numRead=read(fd, response.content, sizeof(response.content)))>0)
	{
		if(mq_send(client_mqd, (char *)&response, sizeof(response), 0)==-1){
			perror("mq_send 1");
			break;
		}

		memset(response.content, '\0', sizeof(response.content));
	}
	response.type = RESP_TYPE_END;
	// strcpy(response.content, "");
	// response.content[0] = '\0';
	if(mq_send(client_mqd, (char *)&response, sizeof(response), 0)==-1){
		perror("mq_send 2");
	}

	return;
}
int
main(int argc, char *argv[])
{
	// atexit(cleanup);
	signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

	// Open server's msg queue:
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(struct req);
	mqd_t server_mqd = mq_open(SERVERMQ_NAME, O_CREAT|O_EXCL|O_RDWR, 0660, &attr);
	if(server_mqd==(mqd_t)-1)
	{
		perror("mq_open");
		exit(EXIT_FAILURE);
	}
	// Change the handler of SIGCHLD to reap terminated children
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	act.sa_handler = grimReaper;
	if(sigaction(SIGCHLD, &act, NULL)==-1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	// Read a request, fork a child, serve the request. break the loop and terminate this server if error occurs. 
	struct req buffer;
	pid_t pid;
	for(;;)
	{
		if(mq_receive(server_mqd, (char *)&buffer, sizeof(buffer), 0)==-1)
		{
			if(errno==EINTR)
				continue;
			perror("mq_receive");
			break;
		}
		
		pid = fork();
		if(pid==-1)
			break;
		else if(pid==0)
		{
			serveRequest(&buffer);
			_exit(EXIT_SUCCESS);
		}
		// else
		// {
			// parent continues on looping and forking
		// }
	}
	
	exit(EXIT_SUCCESS);
}