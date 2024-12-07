/*
 * isock_server : listen on an internet socket ; fork ;
 *                child does lookup ; replies down same socket
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
	static struct sockaddr_in6 server, client;
	socklen_t len = sizeof(client);
	int sd,cd,n;
	Dictrec tryit;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(1);
	}

	/* Create the socket.
	 * Fill in code. */
	if((sd=socket(AF_INET6, SOCK_STREAM, 0))==-1){
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/* Initialize address.
	 * Fill in code. */
	memset(&server, 0, sizeof(server));
	server.sin6_family = AF_INET6;
	server.sin6_port = htons(PORT);
	server.sin6_addr = in6addr_any;
	if(bind(sd, (struct sockaddr *)&server, sizeof(server))==-1){
		perror("bind");
		exit(EXIT_FAILURE);
	}

	/* Name and activate the socket.
	 * Fill in code. */
	if(listen(sd, 128)==-1){
		perror("listen");
		exit(EXIT_FAILURE);
	}

	/* main loop : accept connection; fork a child to have dialogue */
	for (;;) {

		/* Wait for a connection.
		 * Fill in code. */
		cd = accept(sd, (struct sockaddr *)&client, &len);
		if(cd==-1){
			perror("accept");
			exit(EXIT_FAILURE);
		}

		/* Handle new client in a subprocess. */
		switch (fork()) {
			case -1 : 
				DIE("fork");
			case 0 :
				close (sd);	/* Rendezvous socket is for parent only. */
				/* Get next request.
				 * Fill in code. */
				while (recv(cd, (void *)&tryit, sizeof(tryit), 0) > 0) {
					/* Lookup the word , handling the different cases appropriately */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to the client. */
						case FOUND:
							/* Fill in code. */
							if(send(cd, (void *)&tryit, sizeof(tryit), 0)==-1){
								perror("send");
								exit(EXIT_FAILURE);
							}
							break;
						case NOTFOUND:
							/* Fill in code. */
							strcpy(tryit.text, "XXXX");
							if(send(cd, (void *)&tryit, sizeof(tryit), 0)==-1){
								perror("send");
								exit(EXIT_FAILURE);
							}
							 break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */
				} /* end of client dialog */
				exit(0); /* child does not want to be a parent */

			default :
				close(cd);
				break;
		} /* end fork switch */
	} /* end forever loop */
} /* end main */
