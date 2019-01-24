///************* UDP CLIENT CODE *******************/
//
//#include <stdio.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <string.h>
//
//int main(){
//    int clientSocket, portNum, nBytes;
//    char buffer[1024];
//    struct sockaddr_in serverAddr;
//    socklen_t addr_size;
//
//    /*Create UDP socket*/
//    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
//
//    /*Configure settings in address struct*/
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(7891);
//    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
//
//    /*Initialize size variable to be used later on*/
//    addr_size = sizeof serverAddr;
//
//    while(1){
//        printf("Type a sentence to send to server:\n");
//        fgets(buffer,1024,stdin);
//        printf("You typed: %s",buffer);
//
//        nBytes = strlen(buffer) + 1;
//
//        /*Send message to server*/
//        sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
//
//        /*Receive message from server*/
//        nBytes = recvfrom(clientSocket,buffer,1024,0,NULL, NULL);
//
//        printf("Received from server: %s\n",buffer);
//
//    }
//
//    return 0;
//}
// udp client driver program
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include "pthread.h"

#define PORT 5000
#define MAXLINE 1000

void* ReceiveData(void *fd)
{
    long handler = (long) fd;
    while (1) {
        char rbuffer[100];
        recvfrom(handler, rbuffer, 100, 0, (struct sockaddr*)NULL, NULL);
        puts(rbuffer);
        printf("\n Client Recieved message: %s \n", rbuffer);
        bzero(rbuffer, strlen(rbuffer));
    }
    
}

// Driver code
int main()
{
    char mybuffer[100];
    char buffer[100];
    char *message = "Hello Server";
    int sockfd, n;
    struct sockaddr_in servaddr;
    
    // clear servaddr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;
    
    // create datagram socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // connect to server
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }
    
    // request to send datagram
    // no need to specify server address in sendto
    // connect stores the peers IP and port
    pthread_t th;
    pthread_create(&th, NULL, ReceiveData, (void *)sockfd);
    
    while (1) {
        printf("Type a sentence to send to server:\n");
        fgets(mybuffer,100,stdin);
        int nBytes = strlen(buffer) + 1;
        sendto(sockfd, mybuffer, 100, 0, (struct sockaddr*)NULL, sizeof(servaddr));
        
        // waiting for response
        //recvfrom(sockfd, buffer, 100, 0, (struct sockaddr*)NULL, NULL);
        //puts(buffer);
        
    }
    
    // close the descriptor
    close(sockfd);
}
