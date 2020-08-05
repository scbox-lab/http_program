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

struct Messages{
    u_int32_t message_length;
    u_int32_t message_type;
    char data[128];
};
#define MAX_SIZE 1024
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
    //send buf
    struct Messages message;
    for(int i = 0; i < 127; ++i){
        message.data[i] = i;
    }
    message.message_type = 1;
    message.message_length = sizeof(message.data);
    printf("data length = %d\n",  message.message_length);
    char buf[MAX_SIZE];
    bzero(&buf, sizeof(buf));
    memcpy(&buf, &message, sizeof(Messages));
    //send data
    while(true){
        std::string str_input;
        std::cin >> str_input;
        if(str_input == "q"){
            break;
        }
        else if(str_input == "send"){
            if(send(sockfd, (char*)&buf, sizeof(Messages) ,0) < 0){
                perror("send");
            }
        }
        else{
            std::cout << "input msg error" << '\n';
        }
        
    }
    exit(0);
}
