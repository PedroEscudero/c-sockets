#include "restful/http.h"

using namespace std;

int main() {

    unsigned short port = 1500u;
    int server_socket;
    Http_Socket http_socket(port);
    
    fprintf(stderr, "Listening on port %d \n", port);
    http_socket.listen();
    return 0;
}