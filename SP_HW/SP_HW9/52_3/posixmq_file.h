#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <mqueue.h>
#include <errno.h>

#define SERVERMQ_NAME "/myserver_mq_B103040009"

#define CLIENTMQ_NAME_TEMPLATE "/myclient%d_mq_B103040009"

struct req{
	char clientmq_name[30];
	char filename[30];
};
struct res{
	int type;
	char content[128];
};
#define RESP_TYPE_FAILURE 1
#define RESP_TYPE_DATA    2
#define RESP_TYPE_END     3