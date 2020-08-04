#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <string>
#include <sys/stat.h>
#include <pthread.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

#define MAX_SIZE 1024

void read_data(int sockfd, char* buf, size_t& len);
void send_data(int sockfd, char* buf, size_t len);

int main(){
    //socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("socket");
    }
    //socket addr 
    struct sockaddr_in name;
    bzero(&name, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(12345);
    name.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //connect
    if(connect(sockfd, (struct sockaddr*)(&name), sizeof(name)) < 0){
        perror("connect");
    }
    //send data
    char* msg = "Hello World!";
    auto len = strlen(msg);    
    send_data(sockfd, msg, len);
    char buf[MAX_SIZE];
    read_data(sockfd, buf, len);
    printf("%s\n", buf);
    close(sockfd);
    return 0;
}

void read_data(int sockfd, char* buf, size_t& len){
    len = recv(sockfd, buf, MAX_SIZE, 0);
    if(len <= 0){
        return;
    }
}

void send_data(int sockfd, char* buf, size_t len){
    if(send(sockfd, buf, len, 0) <= 0){
        perror("send");
    }
}