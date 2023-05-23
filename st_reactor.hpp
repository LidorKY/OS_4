#pragma once
#include <iostream>
#include <unordered_map>
#include "server.hpp"
#include <vector>
#include <thread>

using namespace std;

class st_reactor
{

public:
    vector<int> fds;
    unordered_map<int, handler_t> hashmap;
    thread reactorThread;
    st_reactor();
    ~st_reactor();
    void* createReactor();
    void stopReactor(st_reactor* reactor);
    void startReactor(st_reactor* reactor);
    void addFd(int fd, handler_t handler);
    void waitFor(st_reactor* reactor);
    void reactorLoop(st_reactor* reactor);
};