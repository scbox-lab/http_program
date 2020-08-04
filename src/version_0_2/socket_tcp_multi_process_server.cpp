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

#define MAX_SIZE 1024

void read_data(int sockfd, char* buf, size_t& len);
void send_data(int sockfd, char* buf, size_t len);

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
    signal(SIGCHLD, SIG_IGN);
    //client
    auto sockfd_client = 0;
    struct sockaddr_in addr_client;
    socklen_t len_client = sizeof(addr_client);

    //
    char buf[MAX_SIZE];
    bzero(buf, MAX_SIZE);
    while(true){
        sockfd_client = accept(sockfd_server, (struct sockaddr*)(&addr_client), &len_client);
        if(sockfd_client < 0){
            perror("accept");
        }
        if(fork() == 0){
            size_t len = 0;
            read_data(sockfd_client, buf, len);
            printf("%s\n", buf);
            send_data(sockfd_client, buf, len);
            bzero(buf, MAX_SIZE);
            close(sockfd_client); 
            exit(0);
        }
        else{
            close(sockfd_client); 
        }        
    }
    
    //close
    close(sockfd_server);
    return 0;
}

void read_data(int sockfd, char* buf, size_t& len){
    len = recv(sockfd, buf, MAX_SIZE, 0);
    buf[len] = '\0';
    if(len <= 0){
        return;
    }
}

void send_data(int sockfd, char* buf, size_t len){
    if(send(sockfd, buf, len, 0) <= 0){
        perror("send");
    }
}