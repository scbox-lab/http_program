#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

#define MAX_SIZE 1024

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

    //client
    auto sockfd_client = 0;
    struct sockaddr_in addr_client;
    socklen_t len_client = sizeof(addr_client);
    //read_buf
    char buf[MAX_SIZE];

    //
    sockfd_client = accept(sockfd_server, (struct sockaddr*)(&addr_client), &len_client);
    if(sockfd_client < 0){
        perror("accept");
    }
    //read
    while(true){
        if(read(sockfd_client, buf, MAX_SIZE-1) == 0){
            break;            
        }
        else{
            printf("receive data from client:\t");
            printf("%s\n", buf);
        }
    }        
    //close
    close(sockfd_client);  

    return 0;
}