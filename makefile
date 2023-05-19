CC=gcc
CXX=g++
CFLAGS=-Wall -g
CXXFLAGS=-Wall -g
LDFLAGS=-ldl -lstdc++

all: react_server

react_server: server.o st_reactor.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c $<

st_reactor.o: st_reactor.cpp
	$(CXX) $(CXXFLAGS) -fPIC -c $<

clean:
	rm -f react_server *.o *.so