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

    //open the camera
    VideoCapture capture(0, CAP_V4L2);
    if(!capture.isOpened()){
        printf("failed to open the video file\n");
        return -1;
    }

    Mat frame;
    vector<uchar> buffer;
    int bufferSize;
    while(1){
        //obatain a frame
        capture >> frame;
        //resize the resolution
        resize(frame, frame, Size(FRAME_WIDTH, FRAME_HEIGHT));

        //encode the obtained frame  
        imencode(".png", frame, buffer);
        bufferSize = buffer.size();

        //send the size of the buffer and the frame
        send(clientSocket, &bufferSize, sizeof(int), 0);

        int bytesSend = 0;
        while(bytesSend < bufferSize){
            int ret   = send(clientSocket, buffer.data() + bytesSend, bufferSize - bytesSend, 0);
            bytesSend = bytesSend + ret;
        }
    }

    close(clientSocket);

    return 0;
}