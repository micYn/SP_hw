/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include "dict.h"

int main(int argc, char **argv) {
    struct sockaddr_un server, client;
    socklen_t clientlen = sizeof(client);
    int sd,cd,n;
    Dictrec tryit;

    if (argc != 3) {
      fprintf(stderr,"Usage : %s <dictionary source>"
          "<Socket name>\n",argv[0]);
      exit(errno);
    }

    /* Setup socket.
     * Fill in code. */
    if((sd=socket(AF_UNIX, SOCK_STREAM,0))==-1){
    	perror("socket");
    	exit(EXIT_FAILURE);
    }
    
    /* Initialize address.
     * Fill in code. */
    memset(&server, 0, sizeof(server));
    server.sun_family = AF_UNIX;
    strncpy(server.sun_path, argv[2], sizeof(server.sun_path)-1);
    if(remove(argv[2])==-1 && errno!=ENOENT){
    	perror("remove");
    	exit(EXIT_FAILURE);
    }
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
		    if((cd=accept(sd, (struct sockaddr *)&client, &clientlen))==-1){
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

							/* Lookup request. */
							switch(lookup(&tryit,argv[1]) ) {
								/* Write response back to client. */
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

						/* Terminate child process.  It is done. */
						exit(0);

					/* Parent continues here. */
					default :
						close(cd);
						break;
				} /* end fork switch */
    } /* end forever loop */
} /* end main */
