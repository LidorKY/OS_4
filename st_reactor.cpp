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
#include <pthread.h>
#include "st_reactor.hpp"
#include "server.hpp"
handler_t listener_handler(int fd) {}
handler_t client_handler(int fd) {}

st_reactor::st_reactor()
{
    // Initialize any data members of the reactor class
    // ...
}

void *st_reactor::createReactor()
{
    this->pfd.clear();
    this->myHashTable.clear();
    return this;
}

void st_reactor::stopReactor(void *reactor)
{
    // Implementation for stopping the reactor
    // You can leave this for now
}

void st_reactor::theThreadFunc(void *reactor)
{
    st_reactor *reactorObj = static_cast<st_reactor *>(reactor);
    char buffer[1024];
    while (true)
    {
        int poll_count = poll(this->pfd.data(), this->pfd.size(), -1);
        if (poll_count == -1)
        {
            perror("error in poll\n");
        }

        for (int i = 0; i < this->pfd.size(); i++)
        {
            if (this->pfd[i].revents & POLLIN)
            {
                if (i == 0)
                {
                    // initialize the socket for communicating with the Sender.
                    struct sockaddr_in new_addr;
                    int client_socket; // the socket
                    socklen_t addr_size = sizeof(new_addr);
                    client_socket = accept(this->pfd[0].fd, (struct sockaddr *)&new_addr, &addr_size); // the func return socket descriptor of a new
                                                                                                       // socket and information of the Sender like IP and Port into new_addr.
                    pollfd listener_fd;
                    listener_fd.fd = client_socket;
                    listener_fd.events = POLLIN;
                    this->pfd.push_back(listener_fd);                                                  // push to vector
                    this->myHashTable[client_socket] = reinterpret_cast<handler_t>(&listener_handler); // push to hashmap
                }
                else
                {
                    bzero(buffer, 1024);
                    ssize_t received = recv(this->pfd[i].fd, buffer, 1024, 0);
                    printf("%s", buffer);
                }
            }
        }
    }
}

void *st_reactor::threadRunner(void *reactor)
{
    static_cast<st_reactor *>(reactor)->theThreadFunc(reactor);
    return NULL;
}

void st_reactor::startReactor(void *reactor)
{
    pthread_create(&myThread, NULL, &st_reactor::threadRunner, reactor);
}

void st_reactor::addFd(void *reactor, int fd, handler_t handler)
{
    st_reactor *reactorObj = static_cast<st_reactor *>(reactor);

    // Add your implementation here
}

void st_reactor::WaitFor(void *freeze_main)
{
    pthread_t *mainThread = static_cast<pthread_t *>(freeze_main);
    pthread_join(*mainThread, NULL);
    printf("Main function is resumed\n");
}