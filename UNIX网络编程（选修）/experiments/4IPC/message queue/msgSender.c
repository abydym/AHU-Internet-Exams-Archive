#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>

struct msg_st{
    long int msg_type;
    char buff[BUFSIZ];
};

int main(int argc, char ** argv){
    struct msg_st msg;
    char buff[BUFSIZ];
    long int msgtype = 0;

    //create the message queue
    int msgid = msgget((key_t)5312, 0666 | IPC_CREAT);
    if(msgid == -1){
        printf("msgget failed\n");
        return -1;
    }

    //send the message to the queue until the message is quit
    while(1){
        printf("Input message: ");
        fgets(buff, BUFSIZ, stdin);
        buff[strlen(buff) - 1] = '\0';
        msg.msg_type = 1;
        strcpy(msg.buff, buff);

        //send the message to the queue
        if(msgsnd(msgid, (void *)&msg, BUFSIZ, 0) == -1){
            printf("msgsnd failed\n");
            return -1;
        }

        if(strcmp(buff, "quit") == 0)
            break;
    }

    return 0;
}