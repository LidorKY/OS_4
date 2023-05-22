#pragma once
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
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <pthread.h>
#include <poll.h>
// #include "server.hpp"
typedef void (*handler_t)(int fd);

using namespace std;

class st_reactor
{
private:
    pthread_t myThread;
    bool stopFlag;

public:
    unordered_map<int, handler_t> myHashTable;
    vector<pollfd> pfd;

    st_reactor();
    void *createReactor();
    void stopReactor(void *reactor);
    void startReactor(void *reactor);
    void WaitFor(void *freeze_main);
    void theThreadFunc(void *reactor);
    static void *threadRunner(void *reactor);
    void addFd(void *reactor, int fd, handler_t handler);
};
