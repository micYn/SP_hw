/*
 * lookup9 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet UDP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in6 server;
	struct hostent *host;
	static int first_time = 1;

	if (first_time) {  /* Set up server address & create local UDP socket */
		first_time = 0;

		/* Set up destination address.
		 * Fill in code. */
		memset(&server, 0, sizeof(server));
		server.sin6_family = AF_INET6;
		server.sin6_port = htons(PORT);
		if(inet_pton(AF_INET6, resource, &server.sin6_addr)==-1){
			perror("inet_pton");
			exit(EXIT_FAILURE);
		}

		/* Allocate a socket.
		 * Fill in code. */
		if((sockfd=socket(AF_INET6, SOCK_DGRAM, 0))==-1){
			perror("socket");
			exit(EXIT_FAILURE);
		}
	}

	/* Send a datagram & await reply
	 * Fill in code. */
	ssize_t numBytes;
	socklen_t len = sizeof(server);
	if(sendto(sockfd, (void *)sought, sizeof(Dictrec), 0, (struct sockaddr *)&server, len)==-1){
		perror("sendto");
		exit(EXIT_FAILURE);
	}
	numBytes = recvfrom(sockfd, (void *)sought, sizeof(Dictrec), 0, (struct sockaddr *)&server, &len);
	if(numBytes==-1){
		perror("recvfrom");
		exit(EXIT_FAILURE);
	}	
	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
