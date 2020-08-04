#include <stdio.h>
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

#define MAXSIZE 1024

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
    const char* msg = "Hello server!";
    auto len_msg = strlen(msg);
    if(write(sockfd, msg, len_msg) <= 0){
        perror("write");
    }
    return 0;
}