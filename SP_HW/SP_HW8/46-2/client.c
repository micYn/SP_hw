#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "seqnum.h"

int main(int argc, char *argv[]){
    struct message msg;
    int msgid;
    int increment = (argc > 1) ? atoi(argv[1]) : 1;

    // Connect to the message queue created by the server
    msgid = msgget(SERVER_KEY, 0620);
    if (msgid == -1) {
        perror("msgget - client");
        exit(1);
    }

    // Prepare request
    msg.mtype = REQ_MSG_TYPE;
    msg.seq_num = getpid();
    msg.request_value = increment;

    // Send the request to the server
    if (msgsnd(msgid, &msg, MSG_SIZE, 0) == -1) {
        perror("msgsnd - client");
        exit(1);
    }

    // Receive the response (use PID as mtype for unique response)
    if (msgrcv(msgid, &msg, MSG_SIZE, getpid(), 0) == -1) {
        perror("msgrcv - client");
        exit(1);
    }

    printf("Received sequence number: %d\n", msg.seq_num);
    return 0;
}
