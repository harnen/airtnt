
CPP_FLAGS = -lboost_system -lpthread -I. -std=c++11 -fPIC -Isample_libcrypto
LINK_FLAGS = -lsample_libcrypto -Lsample_libcrypto

all: server

server: server.cpp ecp.cpp ias_ra.cpp network_ra.cpp service_provider.cpp crypto.cpp
	g++ -o server server.cpp ecp.cpp ias_ra.cpp network_ra.cpp service_provider.cpp $(CPP_FLAGS) $(LINK_FLAGS)

clean: 
	rm server

run: server
	./server