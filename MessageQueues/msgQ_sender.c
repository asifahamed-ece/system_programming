// We will write two programs: a Sender and a Receiver.
// The Sender will send two messages with different "types" (priorities).
// The Receiver will specifically ask for the high-priority message first,
// proving that Message Queues support priority-based retrieval, 
// unlike Pipes which are strictly First-In-First-Out (FIFO).

#include <stdio.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define msg_key 0x1234

// Message Struct
struct msgbuf {
    long mtype; // Always long and > 0
    char mtext[100];
};

int main()
{
    int msqid;
    struct msgbuf msg;

    // 1. Create a Message Queue
    msqid = msgget(msg_key, IPC_CREAT | 0666);
    if(msqid == -1){
        perror("msgget failed!\n");
        return 1;
    }
    printf("[Sender]: Message Queue Created with ID: %d.\n", msqid);

    // 2. Send Message 1 with Low Priority (P = 1)
    msg.mtype = 1; // Type => Priority
    strcpy(msg.mtext, "Sensor Data : Sound = 45dB.");
    //Size is Payload Only!
    if(msgsnd(msqid, &msg, sizeof(msg.mtext), 0) == -1){
        perror("msgsnd failed!\n");
        return 2;
    }
    printf("\n[Sender]: Sent Type 1 (Low Priority): %s\n", msg.mtext);

    // 3. Sending a High priority message
    msg.mtype = 4; // High priority
    strcpy(msg.mtext, "Noise Warning !!!\nSensor Data : Sound = 105dB.");
    if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0) == -1){
        perror("msgsnd failed!\n");
        return 3;
    }
    printf("\n[Sender]: Sent Type 4 (High Priority): %s\n", msg.mtext);


    printf("\n[Sender]: Done Sending Both Data. Waiting for Receiver to Process.\n");
    return 0;
}