#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 55316

int main(){
    char buff[1024];

    //create the socket
    int socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socketServer == -1){
        printf("create socket failed\n");
        return -1;
    }

    //bind the socket
    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family      = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port        = htons(PORT);

    if(bind(socketServer, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1){
        printf("bind failed, errno:");
        return -1;    
    }

    //convert the state into listen state
    if(listen(socketServer, 5) == -1){
        printf("listen failed\n");
        return -1;
    }

    //wait for connection
    struct sockaddr_in cliAddr;
    socklen_t len;

    printf("waiting for connection...\n");

    int conn = accept(socketServer, (struct sockaddr *)(&cliAddr), &len);
    if(conn == -1){
        printf("accept failed\n");
        return -1;
    }

    printf("the client has connected to the server.\n");

    while(1){
        bzero(buff, sizeof(buff));
        read(conn, buff, sizeof(buff));

        if(strcmp(buff, "quit") == 0)
            break;

        else{
            printf("\nshell command: %s\n", buff);
            printf("results of shell command execution: \n");
            system(buff);
        }
    }

    close(socketServer);
    close(conn);
}