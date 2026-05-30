#include <stdio.h>
#include <stdlib.h>
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

        pid_t pid;

        if((pid = fork()) == 0){
            close(socketServer);

            printf("connection from %s, port %d, conn %d is handled by process: %d\n", inet_ntop(AF_INET, &cliAddr.sin_addr, buff, sizeof(buff)), ntohs(cliAddr.sin_port), conn, getpid());

            while(1){
                bzero(buff, sizeof(buff));
                read(conn, buff, sizeof(buff));
                printf("Message from client pid(%d): %s\n", getpid(), buff);
                
                buff[strlen(buff) - 1] = '\0';
                if(strcmp(buff, "quit") == 0)
                    break;
                buff[strlen(buff) - 1] = '\n';


                write(conn, buff, strlen(buff));
            }

            close(conn);
            exit(0);
        }

        close(conn);
    }
    
    close(socketServer);
    printf("the socket is closed\n");
    return 0;
}