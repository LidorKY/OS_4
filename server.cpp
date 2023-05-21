#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <time.h>
#include <poll.h>
#include "server.hpp"
#include "st_reactor.hpp"
#include <pthread.h>
#include "dlfcn.h"
#include <string.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <thread>

handler_t listener_handler(int fd) {}
handler_t client_handler(int fd) {}
int main()
{
    int receiver_socket;
    receiver_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (receiver_socket == -1)
    {
        printf("-there is a problem with initializing receiver\n");
    }
    else
    {
        printf("-initialize successfully.\n");
    }

    int optval = 1;

    if (setsockopt(receiver_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }
    //--------------------------------------------------------------------------------
    // initialize where to send
    struct sockaddr_in Sender_address, new_addr;
    Sender_address.sin_family = AF_INET;
    Sender_address.sin_port = htons(9034);
    Sender_address.sin_addr.s_addr = INADDR_ANY;
    //---------------------------------------------------------------------------------
    // connecting the Receiver and Sender
    int bindd = bind(receiver_socket, (struct sockaddr *)&Sender_address, sizeof(Sender_address));
    if (bindd == -1)
    {
        printf("-there is a problem with binding.\n");
    }
    else
    {
        printf("-binding successfully.\n");
    }
    //---------------------------------------------------------------------------------

    int sock_queue = listen(receiver_socket, 1); // now it can listen to two senders in parallel.
    if (sock_queue == -1)
    { // if there are already 2 senders.
        printf("-queue is full, can't listen.\n");
    }
    else
    {
        printf("-listening...\n");
    }

    // // initialize the socket for communicating with the Sender.
    // int client_socket; // the socket
    // socklen_t addr_size = sizeof(new_addr);
    // client_socket = accept(receiver_socket, (struct sockaddr *)&new_addr, &addr_size); // the func return socket descriptor of a new
    // // socket and information of the Sender like IP and Port into new_addr.
    //---------------------------------------------------------------------------------

    st_reactor reactor;
    void *reactorPtr = reactor.createReactor();

    // Start the reactor thread
    reactor.startReactor(reactorPtr);

    // Run some other code in the main thread
    // ...

    // Wait for the reactor thread to complete
    // pthread_t mainThread = pthread_self();
    // void *mainThreadPtr = static_cast<void *>(&mainThread);
    reactor.WaitFor(reactorPtr);

    return 0;
}