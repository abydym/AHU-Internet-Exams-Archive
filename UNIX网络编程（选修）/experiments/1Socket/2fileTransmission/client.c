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
    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocket == -1){
        printf("failed to create the socket\n");
        return -1;
    }

    //connect to the server
    struct sockaddr_in serverAddressSturcture;
    bzero(&serverAddressSturcture, sizeof(serverAddressSturcture));
    serverAddressSturcture.sin_family      =AF_INET;
    serverAddressSturcture.sin_addr.s_addr = inet_addr("192.168.1.105");
    serverAddressSturcture.sin_port        = htons(COMMUNICATION_PORT);

    if(connect(clientSocket, (struct sockaddr *)&serverAddressSturcture, sizeof(serverAddressSturcture)) == -1){
        printf("failed to connect to the server\n");
        return -1;
    }

    //open the file
    FILE * filePointer = fopen("2.tif", "rb");
    if(filePointer == NULL){
        printf("failed to open the file\n");
        return -1;
    }
    
    //send the file
    char buffer[BUFFER_SIZE];
    int bytesRead = 0;
    while((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, filePointer)) > 0)
        write(clientSocket, buffer, bytesRead);

    close(clientSocket);
    fclose(filePointer);

    return 0;
}