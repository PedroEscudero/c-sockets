#define _XOPEN_SOURCE 1000

#include <sys/socket.h>
#include <string>
#include <iostream>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    struct protoent *protoent;
    struct sockaddr_in client_address, server_address;
    unsigned short port = 12345u;
    int enable = 1;
    socklen_t client_len;
    ssize_t nbytes_read;
    int newline_found = 0;
    char buffer[BUFSIZ];
    int i;
    int server_socket, client_sockfd;
    
    protoent = getprotobyname("tcp");
    server_socket = socket(AF_INET, SOCK_STREAM, protoent->p_proto);
    if(errno){
        printf(" Socket error %d \n", errno);
        exit(EXIT_FAILURE);
    }
    
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
   
    if(errno){
        printf(" Socket error %d \n", errno);
        exit(EXIT_FAILURE);
    }
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    
   bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    
   listen(server_socket, 5);
    
   if(errno){
        printf(" Socket error %d \n", errno);
        exit(EXIT_FAILURE);
    }     

    fprintf(stderr, "listening on port %d\n", port);
    
    while (1) {
        client_len = sizeof(client_address);
        client_sockfd = accept(
            server_socket,
            (struct sockaddr*)&client_address,
            &client_len
        );
        if(errno){
            printf(" Socket clienterror %d \n", errno);
            exit(EXIT_FAILURE);
        }
        while ((nbytes_read = read(client_sockfd, buffer, BUFSIZ)) > 0) {
            printf("received:\n");
            write(STDOUT_FILENO, buffer, nbytes_read);
            if (buffer[nbytes_read - 1] == '\n')
                newline_found;
            for (i = 0; i < nbytes_read - 1; i++)
                buffer[i]++;
            write(client_sockfd, buffer, nbytes_read);
            if (newline_found)
                break;
        }
        close(client_sockfd);
    }
    
    return 0;
}