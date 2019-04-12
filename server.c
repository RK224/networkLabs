#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAXPENDING 5
#define BUFFERSIZE 32

void upperCase(char* msg){
    int n = strlen(msg);
    for(int i=0;i<n;i++){
        msg[i] = toupper(msg[i]);
    }
}

void roundUp(char* msg){
    int d = (int)ceil(atof(msg));
    sprintf(msg,"%d",d);
}

int main(void){
    int serverSocket = socket(PF_INET,SOCK_STREAM,0);
    if(serverSocket < 0){
        printf("Error while creating server socket\n");
        exit(0);
    }
    printf("Server socket created\n");

    struct sockaddr_in serverAddress,clientAddress;
    memset(&serverAddress,0,sizeof(struct sockaddr_in));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    serverAddress.sin_addr.s_addr = htons(INADDR_ANY);
    printf("Server address assigned\n");

    int temp = bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)); 
    if(temp < 0){
        printf("Error while binding\n");
        exit(0);
    }
    printf("Binding successful\n");
    
    int temp1 = listen(serverSocket,MAXPENDING);
    if(temp1 < 0 ){
        printf("Error in listen\n");
        exit(0);
    }
    printf("Now listening\n");
    
    char msg[BUFFERSIZE];
    int clientLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket,(struct sockaddr*) &clientAddress,&clientLength);

    if(clientLength < 0 ){
        printf("Error in client socket\n");
        exit(0);
    }
    printf("Handling client %s\n",inet_ntoa(clientAddress.sin_addr));

    int temp2 = recv(clientSocket,msg,BUFFERSIZE,0);
    if(temp2 < 0){
        printf("Problem in temp2\n");
        exit(0);
    }
    msg[temp2] = '\0';
    printf("%s\n",msg);

   roundUp(msg);    

    int bytesSent = send(clientSocket,msg,strlen(msg),0);
    if(bytesSent != strlen(msg)){
        printf("Error while sending messages\n");
        exit(0);
    }
    close(serverSocket);
}

