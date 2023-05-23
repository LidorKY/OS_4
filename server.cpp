#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "sys/stat.h"
#include "string.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "unistd.h"
#include "netinet/in.h"
#include "netinet/tcp.h"
#include <time.h>
#include <poll.h>
#include "st_reactor.hpp"

#define PORT 9034
void got_message(int fd)
{
    // Handle the received message from the client
    // ...
}
pthread_key_t reactor_key;
void accept_client(int fd)
{
    // Accept the client connection
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(fd, (struct sockaddr*)&client_address, &client_address_len);
    if (client_socket == -1)
    {
        perror("accept");
        return;
    }
    
    // Add the client socket to the reactor
    st_reactor* reactor = static_cast<st_reactor*>(pthread_getspecific(reactor_key));
    if (reactor != nullptr)
    {
        reactor->addFd(client_socket, got_message);
    }
}


int main()
{
    // Create and initialize the reactor
    st_reactor reactor;
    void* Preactor = reactor.createReactor();
    pthread_setspecific(reactor_key, Preactor);

    // Creating a socket
    int receiver_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (receiver_socket == -1)
    {
        perror("socket");
        return 1;
    }

    // Set socket options
    int optval = 1;
    if (setsockopt(receiver_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("setsockopt");
        return 1;
    }

    // Bind the socket to the address
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    if (bind(receiver_socket, (struct sockaddr*)&address, sizeof(address)) == -1)
    {
        perror("bind");
        return 1;
    }

    // Listen for incoming connections
    if (listen(receiver_socket, SOMAXCONN) == -1)
    {
        perror("listen");
        return 1;
    }

    // Add the receiver socket to the reactor
    reactor.addFd(receiver_socket, accept_client);

    // Start the reactor
    reactor.startReactor(&reactor);

    // Wait for the reactor thread to finish
    reactor.waitFor(&reactor);

    // Cleanup and close the server
    reactor.stopReactor(&reactor);
    close(receiver_socket);

    return 0;
}