#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stddef.h>

#define SERVER_KEY 0x1AAAA // Unique key for the message queue
#define REQ_MSG_TYPE 1     // Message type for client requests

// Structure for message queue
struct message {
    long mtype;         // Message type, PID for response
    int seq_num;        // Sequence number (for response from server)
    int request_value;  // Requested increment (for client to server)
};

#define MSG_SIZE (offsetof(struct message, request_value) - offsetof(struct message, seq_num) + sizeof(int))