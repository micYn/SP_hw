/*
 * lookup8 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet TCP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in6 server;
	struct hostent *host;
	static int first_time = 1;

	if (first_time) {        /* connect to socket ; resource is server name */
		first_time = 0;

		/* Set up destination address. */
		server.sin6_family = AF_INET6;
		/* Fill in code. */
		server.sin6_port = htons(PORT);
		if(inet_pton(AF_INET6, resource, &server.sin6_addr)==-1){
			perror("inet_pton");
			exit(EXIT_FAILURE);
		}

		/* Allocate socket.
		 * Fill in code. */
		if((sockfd=socket(AF_INET6, SOCK_STREAM, 0))==-1){
			perror("socket");
			exit(EXIT_FAILURE);
		}

		/* Connect to the server.
		 * Fill in code. */
		if(connect(sockfd, (struct sockaddr *)&server, sizeof(server))==-1){
			perror("connect");
			exit(EXIT_FAILURE);
		}
	}

	/* write query on socket ; await reply
	 * Fill in code. */
	if(send(sockfd, (void *)sought, sizeof(Dictrec), 0)==-1){
		perror("send");
		exit(EXIT_FAILURE);
	}
	if(recv(sockfd, (void *)sought, sizeof(Dictrec), 0)==-1){
		perror("recv");
		exit(EXIT_FAILURE);
	}

	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
