# OS_4


server: 

  creates communication (without accept)
  creates reactor object
  starts the reactors object
  freezes the main (and now only the reacroe's thread is running)

st_reactor:

  has the reactor object
  has a vector for the struct of pfd's (reminder: to use poll you need to define a struct that will contain all the fd's)
  has a hashtable (the first place is for the listener socket of the server in case more people want to jion to the server and its
                  value will be a listener_handler that add the new fd from the accept to the hashtable and the vector)
  createReactor -> only initialize the databases
  stopReactor -> stops the reactor's thread (not relevant for now).
  startReactor -> starts the thread
  addFd -> adds the new fd and its handler function to the hashTable and to the vector
  waitFor -> freezes the main and only the reactor thread will be running
  theThreadFunc -> like in java it is the run method for the thread. it should run in while(1) and run all over the fd's 
                   with the poll method. when the listener fd is "hot" then call addFd. if other fd is "hot" then use
                   recv. if we got 0 bytes then it means that client close the socket then we will close the socket too.
                   if we have got more then 0 bytes then call client_handler and it will just print the message that the client
                   sent to the server. (don't need to do everything else, just print).
  threadRunner -> initialize the thread (there is nothing to touch there).
  
  
  
  
maybe need to fix the makefile so it will run the dynamic library we are compiling 
for example: gcc-o blabla react_server.o ./reactlib.so ->must use the ./reactlib.so
notice: since we work with c++ we need to compile it like c++
  
