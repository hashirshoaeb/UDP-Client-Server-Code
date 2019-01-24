///************* UDP SERVER CODE *******************/
//#include<ctype.h>
//#include <stdio.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <string.h>
//#include <stdlib.h>
//
//int main(){
//    int udpSocket, nBytes;
//    char buffer[1024];
//    struct sockaddr_in serverAddr, clientAddr;
//    struct sockaddr_storage serverStorage;
//    socklen_t addr_size, client_addr_size;
//    int i;
//
//    /*Create UDP socket*/
//    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
//
//    /*Configure settings in address struct*/
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(7891);
//    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
//
//    /*Bind socket with address struct*/
//    bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
//
//    /*Initialize size variable to be used later on*/
//    addr_size = sizeof serverStorage;
//
//    while(1){
//        /* Try to receive any incoming UDP datagram. Address and port of
//         requesting client will be stored on serverStorage variable */
//        nBytes = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);
//        printf("You typed: %s",buffer);
//        /*Convert message received to uppercase*/
//        for(i=0;i<nBytes-1;i++)
//            buffer[i] = toupper(buffer[i]);
//
//        /*Send uppercase message back to client, using serverStorage as the address*/
//        sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
//    }
//
//    return 0;
//}
// server program for udp connection
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include "pthread.h"
#define PORT 5000
#define MAXLINE 1000
char buffer[100];
char mybuffer[100];

struct sockaddr_in servaddr, cliaddr;



void* SendData(void *connfd)
{
    long handler = (long) connfd;
    while (1) {
        //char buffer[100];
        //recv(handler, buffer, 100, 0);
        //sleep(5);
        printf("Type a sentence to send to server:\n");
        fgets(mybuffer,100,stdin);
        int nBytes = strlen(buffer) + 1;
        sendto(handler, mybuffer, 100, 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr));
        printf("\n Server sendingg message: %s ", buffer);
        printf("\n");
        //fflush(*connfd);
    }
}
// Driver code
int main()
{
    //char buffer[100];
    char *message = "Hello Client";
    int listenfd, len;
    //struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(servaddr));
    
    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;
    
    // bind server address to socket descriptor
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    
    pthread_t th;
    pthread_create(&th, NULL, SendData, (void *)listenfd);
    
    while (1) {
        //receive the datagram
        len = sizeof(cliaddr);
        int n = recvfrom(listenfd, buffer, 100, 0, (struct sockaddr*)&cliaddr,&len); //receive message from server
        printf("[+]sent by  %s : %d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
        buffer[n] = '\0';
        puts(buffer);
        
        // send the response
        //int nBytes = strlen(buffer) + 1;
        //sendto(listenfd, buffer, 100, 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr));
        
    }
    
}
