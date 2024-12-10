#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define ROUTER_PATH "/tmp/router"
#define BACKLOG 3
#define BUF_SIZE 128

struct message{
	int sender;
	int receiver;
	char buf[BUF_SIZE];
};

void errorHandler(char *str){
	perror(str);
	exit(EXIT_FAILURE);
}