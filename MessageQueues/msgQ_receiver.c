#include <stdio.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define msg_key 0x1234

//Msg Struct
struct msgbuf {
    long mtype; // always long and > 0.
    char mtext[100];
};

int main()
{
    int msqid;
    struct msgbuf msg;

    // 1. Access the Already Created MsgQueue
    msqid = msgget(msg_key, 0666);
    if(msqid == -1){
        perror("msgget failed!\n");
        return 1;
    }
    printf("[Receiver]: Attached to MsgQueue ID: %d\n", msqid);

    // 2. Lets Receive the HIGH Priority Msg first
    if( msgrcv(msqid, &msg, sizeof(msg.mtext), 4, 0) == -1){
        perror("msgrcv failed!\n");
        return 2;
    }
    printf("\n[Receiver] Msg Type %ld: '%s'\n", msg.mtype, msg.mtext);

    // 3. Receive the next available message irrespectivbe of Type
    if(msgrcv(msqid, &msg, sizeof(msg.mtext), 0, 0) == -1){    // (Type 0 means "give me whatever is next")
        perror("msgrcv failed!\n");
        return 3;
    }
    printf("\n[Receiver] Msg Type %ld: '%s'\n", msg.mtype, msg.mtext);


    // 4. Task Completed. Deleting Msg Queue
    printf("\n[Receiver]: Deleting Message Queue!\n");
    if ( msgctl(msqid, IPC_RMID, NULL) == -1){
        perror("msgctl failed!\n");
        return 4;
    }

    printf("\n[Receiver]: Cleanup Complete. MsgQueue Deleted.\n");
    return 0;
}