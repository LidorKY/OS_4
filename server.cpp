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
#include "st_reactor.hpp"
#include <pthread.h>
#include "dlfcn.h"
#include <string.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <thread>
#include "server.hpp"

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

    int sock_queue = listen(receiver_socket, SOMAXCONN); // now it can listen to two senders in parallel.
    if (sock_queue == -1)
    { // if there are already 2 senders.
        printf("-queue is full, can't listen.\n");
    }
    else
    {
        printf("-listening...\n");
    }

    st_reactor reactor;

    void *reactorPtr = reactor.createReactor();

    pollfd listener_fd;
    listener_fd.fd = receiver_socket;
    listener_fd.events = POLLIN;
    reactor.pfd.push_back(listener_fd); // push to vector

    reactor.myHashTable[receiver_socket] = reinterpret_cast<handler_t>(&listener_handler); // push to hashmap

    reactor.startReactor(reactorPtr); // Start the reactor thread
    reactor.WaitFor(reactorPtr);

    return 0;
}