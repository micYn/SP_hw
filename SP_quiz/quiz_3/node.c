#include "net.h"

int main(int argc, char *argv[]){
	int sfd;
	struct sockaddr_un routeraddr, nodeaddr;
	struct message msg, recvmsg;

	ssize_t numRead;

	if((sfd=socket(AF_UNIX, SOCK_STREAM, 0))==-1)
		errorHandler("socket");

	memset(&routeraddr, 0, sizeof(routeraddr));
	routeraddr.sun_family = AF_UNIX;
	strncpy(routeraddr.sun_path, ROUTER_PATH, sizeof(routeraddr.sun_path)-1);
	if(connect(sfd, (struct sockaddr *)&routeraddr, sizeof(routeraddr))==-1)
		errorHandler("connect");
	printf("connected to server\n");
	sleep(10);
	// while(1){
		int receiver;
		// printf("Send to: ");
		// scanf("%d", &receiver);
		msg.sender = atoi(argv[1]);
		msg.receiver = atoi(argv[2]);
		if(msg.receiver == 0){
			snprintf(msg.buf, BUF_SIZE, "%d sends to all\n", msg.sender);
		}
		else{
			snprintf(msg.buf, BUF_SIZE, "%d sends to %d\n", msg.sender, msg.receiver);
		}

		write(sfd, (void *)&msg, sizeof(struct message));
		printf("wrote\n");
		while((numRead=read(sfd, (void *)&recvmsg, sizeof(struct message)))>0){
			printf("Node %s receives %s\n", argv[1], recvmsg.buf);
			fflush(stdout);
		}
	// }
	

	

}