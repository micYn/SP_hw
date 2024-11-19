#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "seqnum.h"

static int msgid;
static int seq_num = 0;  // Starting sequence number

void cleanup(int sig) {
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl - server");
    }
    exit(0);
}

int main() {
    struct message msg;

    // Create message queue
    msgid = msgget(SERVER_KEY, IPC_CREAT | 0620);
    if (msgid == -1) {
        perror("msgget - server");
        exit(1);
    }

    // Handle termination to clean up message queue
    signal(SIGINT, cleanup);

    // printf("Server started.\n");

    while (1) {
        // Receive client request
        if (msgrcv(msgid, &msg, MSG_SIZE, REQ_MSG_TYPE, 0) == -1) {
            perror("msgrcv - server");
            continue;
        }

        // Prepare response message with updated sequence number
        msg.mtype = msg.seq_num;  // Set to client PID for directed response
        msg.seq_num = seq_num;

        // Increment sequence number
        seq_num += msg.request_value;

        // Send response back to client
        if (msgsnd(msgid, &msg, MSG_SIZE, 0) == -1) {
            perror("msgsnd - server");
            continue;
        }
    }

    return 0;
}
