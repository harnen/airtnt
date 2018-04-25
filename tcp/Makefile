
all: client server

client: client.cpp
	g++ client.cpp -o client  -lboost_system -lpthread

server: server.cpp 
	g++ server.cpp -o server  -lboost_system -lpthread

clean: 
	rm server client


