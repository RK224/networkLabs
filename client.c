#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 32

int main(){
    int sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock < 0){
        printf("Error in opening socket\n");
        exit(0);
    }
    printf("socket created\n");

    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    printf("Address assigned\n");

    int c = connect(sock,(struct sockaddr*) &serverAddr,sizeof(serverAddr));
    printf("%d\n",c);
    if(c < 0){
        printf("Error while establishing a connection\n");
        exit(0);
    }
    printf("Connection established\n");

    printf("ENTER MESSAGE FOR SERVER with max 32 characters:\n");
    char* msg = (char*)malloc(sizeof(char)*BUFSIZE);
    fgets(msg,BUFSIZE,stdin);
    int bytesent = send(sock,msg,strlen(msg),0);
    if(bytesent != strlen(msg)){
        printf("Error while sending message\n");
        exit(0);
    }
    printf("Data sent\n");

    char* recvBuffer = (char*)malloc(sizeof(char)*BUFSIZE);
    int bytesRecvd = recv(sock,recvBuffer,BUFSIZE-1,0);
    if(bytesRecvd < 0){
        printf("Error while receiving data from the server\n");
        exit(0);
    }
    recvBuffer[bytesRecvd] = '\0';
    printf("%s\n",recvBuffer);
    close(sock);
    free(msg);
    free(recvBuffer);
    return 0;
}
