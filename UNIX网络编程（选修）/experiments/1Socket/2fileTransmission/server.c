#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define COMMUNICATION_PORT 55312
#define BUFFER_SIZE        1024

int main(){

    //create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket == -1){
        printf("failed to create the socket\n");
        return -1;
    }

    //initialize the socket
    struct sockaddr_in serverAddressSturcture;
    bzero(&serverAddressSturcture, sizeof(serverAddressSturcture));
    serverAddressSturcture.sin_family      =AF_INET;
    serverAddressSturcture.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddressSturcture.sin_port        = htons(COMMUNICATION_PORT);

    if((bind(serverSocket, (struct sockaddr *) &serverAddressSturcture, sizeof(serverAddressSturcture))) == -1){
        printf("failed to bind the socket\n");
        return -1;
    }

    //convert the state of the socket into listening state
    if((listen(serverSocket, 5)) == -1){
        printf("failed to enter listening state\n");
        return -1;
    }

    //wait for connection
    struct sockaddr_in clientAddressSturcture;
    socklen_t clientAddressSturctureLength;
    int connectionSocket = accept(serverSocket, (struct sockaddr *) &clientAddressSturcture, &clientAddressSturctureLength);
    if(connectionSocket == -1){
        printf("failed to accept the connection from the client\n");
        return -1;
    }

    //open the file
    FILE * filePointer = fopen("receivedFile.tif", "wb");
    if(filePointer == NULL){
        printf("failed to open the file\n");
        return -1;
    }

    //receive the file
    char buffer[BUFFER_SIZE];
    int bytesRead = 0;
    while((bytesRead = read(connectionSocket, buffer, BUFFER_SIZE)) > 0)
        fwrite(buffer, sizeof(char), bytesRead, filePointer);

    close(serverSocket);
    fclose(filePointer);

    return 0;
}

