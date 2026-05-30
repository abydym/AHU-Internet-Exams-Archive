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
    bzero(buff, 1024);
    printf("This is the client\n");

    //create the socket
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if(socketClient == -1){
        printf("create socket failed\n");
        return -1;
    }

    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family      = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("124.222.94.52");
    servAddr.sin_port        = htons(PORT);

    if (connect(socketClient, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1){
        printf("connect failed\n");
        return -1;
    }

    printf("connected to the server\n");
    while(1){
        printf("Input Message: ");
        bzero(buff, sizeof(buff));
        fgets(buff, 1024, stdin);
        write(socketClient, buff, strlen(buff));

        buff[strlen(buff) - 1] = '\0';
        if(strcmp(buff, "quit") == 0)
            break;

        bzero(buff, sizeof(buff));
        read(socketClient, buff, sizeof(buff));
        printf("Message from the server: %s\n\n", buff);
    }
    printf("%s\n", buff);

    close(socketClient);
    printf("sokcet is closed\n");

    return 0;
}