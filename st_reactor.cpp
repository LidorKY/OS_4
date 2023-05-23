#include "st_reactor.hpp"
#include <poll.h>
#include <algorithm>  // For std::remove_if
#include <errno.h>    // For errno
#include <mutex>      // For std::lock_guard, std::mutex
#include <sys/stat.h> // For fstat

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

void st_reactor::updateFds(vector<pollfd>& pollfds)
{
    // Lock the mutex to ensure thread-safety
    std::lock_guard<std::mutex> lock(mutex);

    // Remove any closed file descriptors from the pollfds vector
    pollfds.erase(std::remove_if(pollfds.begin(), pollfds.end(), [this](const pollfd& pfd) {
        return std::find(fds.begin(), fds.end(), pfd.fd) == fds.end();
    }), pollfds.end());

    // Add any new file descriptors from the fds vector to the pollfds vector
    for (int fd : fds)
    {
        auto it = std::find_if(pollfds.begin(), pollfds.end(), [fd](const pollfd& pfd) {
            return pfd.fd == fd;
        });

        if (it == pollfds.end())
        {
            pollfd pfd;
            pfd.fd = fd;
            pfd.events = POLLIN;
            pfd.revents = 0;
            pollfds.push_back(pfd);
        }
    }
}


void st_reactor::reactorLoop(st_reactor* reactor)
{
    cout << "in reactorLoop" << endl;
    vector<pollfd> pollfds(reactor->fds.size());
    while (true)
    {
        cout << "1" << endl;
        // Perform the poll operation on the file descriptors

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
        int count = 0;
        // Check for events on the file descriptors
        for (const auto& pollfd : pollfds)
        {
            cout << "Iter = " << ++count << endl;
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

        // Update the reactor's file descriptors after each iteration
        reactor->updateFds(pollfds);
    }
}

