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

function error_control(int error){
   if(error){
        printf(" Socket error %d \n", errno);
        exit(EXIT_FAILURE);
    } 
}

int main() {
    struct protoent *protoent;
    struct sockaddr_in client_address, server_address;
    unsigned short port = 1500u;
    int is_enable = 1;
    socklen_t client_lenght;
    ssize_t nbytes_read;
    int newline = 0;
    char buffer[BUFSIZ];
    int i;
    int server_socket, client_socket;
    
    protoent = getprotobyname("tcp");
    server_socket = socket(AF_INET, SOCK_STREAM, protoent->p_proto);
    error_control(errno);

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &is_enable, sizeof(is_enable));
    error_control(errno);
   
    if(errno){
        printf(" Socket error %d \n", errno);
        exit(EXIT_FAILURE);
    }
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(port);
    server_address.sin_port = htons(port);
    
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    error_control(errno);
    
    listen(server_socket, 5);
    error_control(errno);
    
    fprintf(stderr, "Listening on port %d\n", port);
    
    while (!errno) {
        client_lenght = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_lenght);
        
        while ((nbytes_read = read(client_socket, buffer, BUFSIZ)) > 0) {
            printf("New message:\n");
            write(STDOUT_FILENO, buffer, nbytes_read);
            if (buffer[nbytes_read - 1] == '\n')
                newline;
            for (i = 0; i < nbytes_read - 1; i++)
                buffer[i]++;
            write(client_sockfd, buffer, nbytes_read);
            if (newline)
                break;
        }
        close(client_socket);
    }
    
    return 0;
}