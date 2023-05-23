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
    stopReactor(this);
}
void* st_reactor::createReactor()
{
    cout << "in createReactor" << endl;
    st_reactor* reactor = new st_reactor;
    return reactor;
}

void st_reactor::stopReactor(st_reactor* reactor)
{
    delete reactor;
}
void st_reactor::startReactor(st_reactor* reactor)
{
    cout << "in startReactor" << endl;
    reactorThread = thread([reactor]() {
        reactor->reactorLoop(reactor);
    });
}

void st_reactor::addFd(int fd, handler_t handler)
{
    cout << "in addFd" << endl;
    fds.emplace_back(fd);
    hashmap.emplace(fd,handler);
}
void st_reactor::waitFor(st_reactor* reactor)
{
    cout << "in waitFor" << endl;
    // Wait for the reactor thread to finish
    pthread_join(reactor->reactorThread.native_handle(), NULL);
}
void st_reactor::reactorLoop(st_reactor* reactor)
{
    cout << "in reactorLoop" << endl;
    while (true)
    {
        cout << "1" << endl;
        // Perform the poll operation on the file descriptors
        vector<pollfd> pollfds(reactor->fds.size());

        for (size_t i = 0; i < reactor->fds.size(); i++)
        {
            pollfds[i].fd = reactor->fds[i];
            pollfds[i].events = POLLIN;
            pollfds[i].revents = 0;
        }
        cout << "2" << endl;
        int numReady = poll(pollfds.data(), pollfds.size(), -1);
        cout << "3" << endl;
        if (numReady == -1)
        {
            perror("poll");
            break;
        }
        cout << "4" << endl;
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
        cout << "5" << endl;
    }
}
