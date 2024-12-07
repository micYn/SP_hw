/*
 * udp_server : listen on a UDP socket ;reply immediately
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in6 server,client;
	int sockfd,siz;
	ssize_t numBytes;
	Dictrec dr, *tryit = &dr;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(errno);
	}

	/* Create a UDP socket.
	 * Fill in code. */
	if((sockfd=socket(AF_INET6, SOCK_DGRAM, 0))==-1){
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/* Initialize address.
	 * Fill in code. */
	memset(&server, 0, sizeof(server));
	server.sin6_family = AF_INET6;
	server.sin6_port = htons(PORT);
	server.sin6_addr = in6addr_any;

	/* Name and activate the socket.
	 * Fill in code. */
	if(bind(sockfd, (struct sockaddr *)&server, sizeof(server))==-1){
		perror("bind");
		exit(EXIT_FAILURE);
	}

	for (;;) { /* await client packet; respond immediately */

		siz = sizeof(client); /* siz must be non-zero */

		/* Wait for a request.
		 * Fill in code. */
		numBytes = recvfrom(sockfd, (void *)tryit, sizeof(Dictrec), 0, (struct sockaddr *)&client, &siz);
		if(numBytes==-1){
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}

		while (numBytes>0) {
			/* Lookup request and respond to user. */
			switch(lookup(tryit,argv[1]) ) {
				case FOUND: 
					/* Send response.
					 * Fill in code. */
					if(sendto(sockfd, (void *)tryit, sizeof(Dictrec), 0, (struct sockaddr *)&client, siz)==-1){
						perror("sendto");
						exit(EXIT_FAILURE);
					}
					break;
				case NOTFOUND : 
					/* Send response.
					 * Fill in code. */
					strcpy(tryit->text, "XXXX");
					if(sendto(sockfd, (void *)tryit, sizeof(Dictrec), 0, (struct sockaddr *)&client, siz)==-1){
						perror("sendto");
						exit(EXIT_FAILURE);
					}
					break;
				case UNAVAIL:
					DIE(argv[1]);
			} /* end lookup switch */
			break;
		} /* end while */
	} /* end forever loop */
} /* end main */
