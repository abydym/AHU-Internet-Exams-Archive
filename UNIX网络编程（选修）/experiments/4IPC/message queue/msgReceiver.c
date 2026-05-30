#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

struct msg_st{
    long int msg_type;
    char buff[BUFSIZ];
};

int main(int argc, char ** argv){
    struct msg_st msg;
    long int msgtype = 0;

    //create the message queue
    int msgid = msgget((key_t)5312, 0666 | IPC_CREAT);
    if(msgid == -1){
        printf("msgget failed\n");
        return -1;
    }

    //fetch message from the queue until the message is quit
    while(1){
        if(msgrcv(msgid, (void *)&msg, BUFSIZ, msgtype, 0) == -1){
            printf("msgrcv failed\n");
            return -1;
        }

        printf("Message from the sender: %s\n", msg.buff);

        if(strcmp(msg.buff, "quit") == 0)
            break;
    }

    //delete the message queue
    if(msgctl(msgid, IPC_RMID, 0) == -1){
        printf("msgctl(IPC_RMID) failed\n");
        return -1;
    }

    return 0;
}