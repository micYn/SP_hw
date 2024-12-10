#include "net.h"

int main(){
	int sfd;
	int cfd[3];
	struct sockaddr_un routeraddr, nodeaddr;
	socklen_t nodeaddrlen = sizeof(nodeaddr);

	int i,j;
	int tmpcfd;
	ssize_t numRead;
	struct message fromnode, tonode;

	if((sfd=socket(AF_UNIX, SOCK_STREAM, 0))==-1)
		errorHandler("socket");

	memset(&routeraddr, 0, sizeof(routeraddr));
	routeraddr.sun_family = AF_UNIX;
	strncpy(routeraddr.sun_path, ROUTER_PATH, sizeof(routeraddr.sun_path)-1);
	if(remove(ROUTER_PATH)==-1 && errno != ENOENT)
		errorHandler("remove");
	if(bind(sfd, (struct sockaddr *)&routeraddr, sizeof(routeraddr))==-1)
		errorHandler("bind");

	if(listen(sfd, BACKLOG)==-1)
		errorHandler("listen");

	for(i=0;i<3;i++){
		cfd[i] = accept(sfd, (struct sockaddr *)&nodeaddr, &nodeaddrlen);
		printf("connected %d\n", cfd[i]);
	}
	while(1){
		for(i=0;i<3;i++){
			while((numRead=read(cfd[i], (void *)&fromnode, sizeof(struct message)))>0){
				
				if(fromnode.receiver == 0){
					tonode.sender = fromnode.sender;
					strcpy(tonode.buf, fromnode.buf);
					printf("%d sends to all\n", fromnode.sender);
					for(j=0;j<i;j++){
						tonode.receiver = j+1;
						if(write(cfd[j], (void *)&tonode, numRead)<numRead)
							errorHandler("write");
					}
				}
				else{
					tonode.sender = fromnode.sender;
					tonode.receiver = fromnode.receiver;
					printf("%d sends to %d\n", fromnode.sender, fromnode.receiver);
					strcpy(tonode.buf, fromnode.buf);
					if(write(cfd[tonode.receiver-1], (void *)&tonode, numRead)<numRead)
							errorHandler("write");
				}
			}
			if(numRead==-1)
				errorHandler("read");
		}
	}
}