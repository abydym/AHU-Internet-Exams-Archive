#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

#define COMMUNICATION_PORT 55316
#define BUFFER_SIZE        1024
#define FRAME_WIDTH        512
#define FRAME_HEIGHT       512

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

    vector<uchar> buffer;
    Mat frame;
    while(1){
        //receive the size of the buffer and the frame
        int bufferSize;
        recv(connectionSocket, &bufferSize, sizeof(int), 0);
        buffer.resize(bufferSize);

        int bytesReceived = 0;
        while(bytesReceived < bufferSize){
            int ret       = recv(connectionSocket, buffer.data() + bytesReceived, bufferSize - bytesReceived, 0);
            bytesReceived = bytesReceived + ret;
        }

        frame = imdecode(buffer, IMREAD_COLOR);
        imshow("received frame", frame);
        waitKey(40);
    }

    close(serverSocket);

    return 0;
}

