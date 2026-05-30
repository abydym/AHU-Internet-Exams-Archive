#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 55312

int main(int argc, const char ** argv){
    char buff[1024];
    printf("This is the server\n");

    //create the socket
    int socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socketServer == -1){
        printf("create socket failed\n");
        return -1;
    }

    printf("the socket is created\n");

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

    printf("the address is bound\n");

    //convert the state into listen state
    if(listen(socketServer, 5) == -1){
        printf("listen failed\n");
        return -1;
    }

    printf("listen state\n");

    //wait for connection
    struct sockaddr_in cliAddr;
    socklen_t len;

    while(1){
        printf("waiting for connection\n");
        int conn = accept(socketServer, (struct sockaddr *)(&cliAddr), &len);
        if(conn == -1){
            printf("accept failed\n");
            return -1;
        }

        printf("connection from %s, port %d\n", inet_ntop(AF_INET, &cliAddr.sin_addr, buff, sizeof(buff)), ntohs(cliAddr.sin_port));

        time_t ticks = time(NULL);
        printf("sending message...");
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(conn, buff, strlen(buff));
        printf("message is sent\n\n");


    }
    

    close(socketServer);
    printf("the socket is closed\n");
    return 0;
}