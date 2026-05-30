#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define COMMUNICATION_PORT 55316

int main(){
    char buff[1024];
    char message[1024];
    
    //create the socket
    int connfd = socket(AF_INET, SOCK_STREAM, 0);
    if(connfd == -1){
        printf("1. Create socket failed\n");
        return -1;
    }

    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family      = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("192.168.1.105");
    servAddr.sin_port        = htons(COMMUNICATION_PORT);

    //connect to the sever
    if (connect(connfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1){
        printf("1. Connect failed\n");
        return -1;
    }

    printf("the client has connected to the server.\n");

    while(1){
        printf("input shell command: ");
        fgets(buff, sizeof(buff), stdin);
        buff[strlen(buff) - 1] = '\0';
        write(connfd, buff, strlen(buff));

        if(strcmp(buff, "quit") == 0)
            break;

    }

    close(connfd);
}
