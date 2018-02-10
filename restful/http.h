#ifndef HTTP
#define HTTP
#define _XOPEN_SOURCE 1200

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

using namespace std;

class Http_Socket{
    
    void error_control(int error){
       if(error){
            cout << " Socket error" << errno;
            exit(EXIT_FAILURE);
        } 
    }
    
    int open(){   
        
        struct protoent *protoent;
        struct sockaddr_in server_address;
        int is_enable = 1;

        protoent = getprotobyname("tcp");
        server_socket = socket(AF_INET, SOCK_STREAM, protoent->p_proto);
        error_control(errno);

        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &is_enable, sizeof(is_enable));
        error_control(errno);

        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        server_address.sin_port = htons(port);

        bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
        error_control(errno);

        ::listen(server_socket, 5);
        error_control(errno);

        return server_socket; 
    }
    
    int listen_in_loop(){

        socklen_t client_lenght;
        ssize_t bytes_read;
        int newline = 0;
        char buffer[BUFSIZ];
        int client_socket;
        struct sockaddr_in client_address;
            
        while (!errno) {
            client_lenght = sizeof(client_address);
            client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_lenght);
            while ((bytes_read = read(client_socket, buffer, BUFSIZ)) > 0) {
                cout << "New message: \n";
                write(STDOUT_FILENO, buffer, bytes_read);
                for (int i = 0; i < bytes_read - 1; i++)
                    buffer[i]++;
                write(client_socket, buffer, bytes_read);
                if (newline)
                    break;
            }
            close(client_socket);
        }
      return 0;  
    }
    
    public:
    int server_socket;
    unsigned short port;
    Http_Socket(unsigned short);  
    
    void listen(){
        this->open();
        this->listen_in_loop();    
    }
};

Http_Socket::Http_Socket(unsigned short a){
    port = a;
}

#endif

