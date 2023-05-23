#include "st_reactor.hpp"
#include <poll.h>
using namespace std;

st_reactor::st_reactor()
{
    fds = vector<int>();
    hashmap = unordered_map<int, handler_t>();
}
st_reactor::~st_reactor()
{
    if (this != nullptr)
    {
        stopReactor(this);
        delete this;
    }
}
void *st_reactor::createReactor()
{
    st_reactor reactor; // Remove the parentheses or use braces
    return &reactor;
}

void st_reactor::stopReactor(st_reactor *reactor)
{
    delete &reactor->fds;
    delete &reactor->hashmap;
}
void st_reactor::startReactor(st_reactor *reactor)
{
    reactorThread = thread(&st_reactor::reactorLoop, reactor);//create a thread and excute it.
}

void st_reactor::addFd(int fd, handler_t handler)
{
    hashmap.emplace(fd,handler);
}
void st_reactor::waitFor(st_reactor* reactor)
{
    // Wait for the reactor thread to finish
    pthread_join(reactor->reactorThread.native_handle(), NULL);
}
void st_reactor::reactorLoop(st_reactor* reactor)
{
    while (true)
    {
        // Perform the poll operation on the file descriptors
        vector<pollfd> pollfds(reactor->fds.size());

        for (size_t i = 0; i < reactor->fds.size(); i++)
        {
            pollfds[i].fd = reactor->fds[i];
            pollfds[i].events = POLLIN;
            pollfds[i].revents = 0;
        }

        int numReady = poll(pollfds.data(), pollfds.size(), -1);

        if (numReady == -1)
        {
            perror("poll");
            break;
        }

        // Check for events on the file descriptors
        for (const auto& pollfd : pollfds)
        {
            int fd = pollfd.fd;

            if (pollfd.revents & POLLIN)
            {
                // Find the corresponding handler for the file descriptor
                auto it = reactor->hashmap.find(fd);
                if (it != reactor->hashmap.end())
                {
                    handler_t handler = it->second;
                    // Call the handler function
                    handler(fd);
                }
            }
        }
    }
}
