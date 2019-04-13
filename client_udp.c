#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512

#define PORT 8888

void die(char* s){
    perror(s);
    exit(1);
}

int main(void){
    struct sockaddr_in si_other;
    int s,i,slen;
    slen = sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(s == -1) die("socket");

    memset(&si_other,0,sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(1){
        printf("Enter message : ");
        fgets(message,strlen(message),stdin);

        int c = connect(s,(struct sockaddr*) &si_other,sizeof(si_other));
        if(c < 0)   printf("Error while establishing connection\n");
        
        int bytesSent = send(s,message,strlen(message),0);
        if(bytesSent != strlen(message)){
            die("send()");
        }

        memset(buf,'\0',BUFLEN);

        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1){
            die("recvfrom()");
        }

        puts(buf);        

    }

    close(s);
    return 0;
}