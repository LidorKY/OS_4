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

st_reactor::st_reactor()
{
    // Initialize any data members of the reactor class
    // ...
}

void *st_reactor::createReactor()
{
    this->myVector.clear();
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
    // Implementation for the thread function
    // This is where you can perform the reactor's functionality
    st_reactor *reactorObj = static_cast<st_reactor *>(reactor);
    // Example code: Loop and print a message
    while (true)
    {
        printf("Thread function is \n");
        // Add your reactor logic here
        // Remember to handle events, dispatch handlers, etc.
        sleep(1); // Delay for 1 second
    }
}

void *st_reactor::threadRunner(void *reactor)
{
    // Call the actual thread function
    static_cast<st_reactor *>(reactor)->theThreadFunc(reactor);
    return NULL;
}

void st_reactor::startReactor(void *reactor)
{
    // Create a new thread and execute the thread function
    pthread_create(&myThread, NULL, &st_reactor::threadRunner, reactor);
    printf("Reactor thread started\n");
}

void st_reactor::addFd(void *reactor, int fd, handler_t handler)
{
    // Implementation for adding a file descriptor and handler
    // to the reactor
    st_reactor *reactorObj = static_cast<st_reactor *>(reactor);

    // Add your implementation here
}

void st_reactor::WaitFor(void *freeze_main)
{
    // Implementation for waiting in the main thread
    // You can leave this for now
    pthread_t *mainThread = static_cast<pthread_t *>(freeze_main);
    pthread_join(*mainThread, NULL);
    printf("Main function is resumed\n");
}