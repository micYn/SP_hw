#include "posixmq_file.h"


int
main(int argc, char *argv[])
{
	if(argc!=2)
	{
		fprintf(stderr, "Usage: %s <filename>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	/***********************************************************************************
	Create and open a client msg queue for read & write before we send the request:	
	************************************************************************************/
	char clientmq_name[256];
	snprintf(clientmq_name, sizeof(clientmq_name), CLIENTMQ_NAME_TEMPLATE, getpid());
	
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(struct res);
	mqd_t client_mqd = mq_open(clientmq_name, O_CREAT|O_EXCL|O_RDWR, 0660, &attr);
	if(client_mqd==(mqd_t)-1)
	{
		perror("mq_open 1");
		exit(EXIT_FAILURE);
	}
	/************************************************************************************/
	

	/************************************************************************************
	Open the server's msg queue to send a request (should already exists):
	*************************************************************************************/
	mqd_t server_mqd = mq_open(SERVERMQ_NAME, O_WRONLY);
	if(server_mqd==(mqd_t)-1)
	{
		perror("mq_open 2");
		exit(EXIT_FAILURE);
	}
	// Send a request to server:
	struct req request;
	strcpy(request.clientmq_name,clientmq_name);		// so the server can use this to access this client's msg queue
	strcpy(request.filename, argv[1]);

	if(mq_send(server_mqd, (char *)&request, sizeof(request), 0)==-1)
	{
		perror("mq_send c");
		exit(EXIT_FAILURE);
	}
	
	if(mq_close(server_mqd)==-1)	// Close the server's msg queue descriptor:
	{
		perror("mq_close");
		exit(EXIT_FAILURE);
	}
	/*************************************************************************************/

	
	/************************************************************************************
	Get response:
	*************************************************************************************/
	ssize_t total_bytes = 0, bytes_received;
	int num_msg;

	struct res response;
	if((bytes_received=mq_receive(client_mqd, (char *)&response, sizeof(response), 0))==-1)
	{
		perror("mq_receive");
		exit(EXIT_FAILURE);
	}
	
	printf("%s", response.content);
	total_bytes += bytes_received;

	if(response.type==RESP_TYPE_FAILURE)
	{
		
		// Delete this client queue:
		if(mq_unlink(clientmq_name)==-1)
		{
			perror("mq_unlink");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_FAILURE);
	}
	
	
	for(num_msg=1;response.type==RESP_TYPE_DATA;num_msg++)
	{
		bytes_received = mq_receive(client_mqd, (char *)&response, sizeof(response), 0);
		if(bytes_received==-1)
		{
			perror("mq_receive");
			exit(EXIT_FAILURE);
		}
		printf("%s", response.content);
		total_bytes += bytes_received;
	}

	printf("\nReceived %ld bytes (%d messages)\n", (long)total_bytes, num_msg);
	/*************************************************************************************/

	// Delete this client queue:
	if(mq_unlink(clientmq_name)==-1)
	{
		perror("mq_unlink");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}