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
    struct sockaddr_in si_me,si_other;
    int s,i,slen,recvLen;
    
    slen = sizeof(si_other);
    char buf[BUFLEN];

    s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(s == -1) die("socket()");

    memset(&si_me,0,sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    int c = bind(s,(struct sockaddr*) &si_me,sizeof(si_me));

    if( c == -1) die("bind()");

    while(1){
        printf("Waiting for data...\n");
        fflush(stdout);

    if ((recvLen = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1){
        die("recvfrom()");
    }

        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n",buf);

        int bytesSent = sendto(s,buf,recvLen,0,(struct sockaddr*) &si_other, slen);
        if(bytesSent != recvLen) die("sendto()");

    }
    close(s);
    return 0 ;
}
