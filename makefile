all: react_server

react_server: server.o libst_reactor.so
	g++ -std=c++11 -Wall -Wextra -o react_server server.o ./libst_reactor.so

server.o: server.cpp
	g++ -std=c++11 -Wall -Wextra -c server.cpp

libst_reactor.so: st_reactor.o
	g++ -shared -o libst_reactor.so st_reactor.o

st_reactor.o: st_reactor.cpp st_reactor.hpp
	g++ -std=c++11 -Wall -Wextra -fPIC -c st_reactor.cpp st_reactor.hpp

clean:
	rm -f *.o *.so *.gch react_server
