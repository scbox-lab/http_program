#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <signal.h>

struct Messages{
    uint32_t message_length;
    uint32_t message_type;
    char data[128];
};

#define  MAX_SIZE 1024

int main(){
    //server
    //socket
    auto sockfd_server = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd_server < 0){
        perror("socket");
    }
    //socket addr
    struct sockaddr_in addr_server;
    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_server.sin_port = htons(12345);
     //bind
    if(bind(sockfd_server, (struct sockaddr*)(&addr_server), sizeof(addr_server)) < 0){
        perror("bind");
    }
    //listen
    if(listen(sockfd_server, 5) < 0){
        perror("listen");
    }
    //signal
    signal(SIGPIPE, SIG_IGN);
    //client
    auto sockfd_client = 0;
    struct sockaddr_in addr_client;
    socklen_t len_client = sizeof(addr_client);
    //
    char buf[MAX_SIZE];
    bzero(&buf, sizeof(buf));
    //
    while(true){
        sockfd_client = accept(sockfd_server, (struct sockaddr*)(&addr_client), &len_client);
        printf("client pid = %d\n", sockfd_client);
        if(sockfd_client < 0){
            perror("accept");
        }
        int n = recv(sockfd_client, buf, sizeof(buf), 0);
        if (n < 0) {
            error(1, errno, "error read message");
        } else if (n == 0) {
            error(1, 0, "client closed \n");
        }
        //解析message
        struct Messages message;
        bzero(&message, sizeof(Messages));
        memcpy(&message, buf, sizeof(Messages));
        printf("%d\n %c\n", message.message_length, message.data[65]);
    }
    exit(0);
}
