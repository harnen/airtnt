
CPP_FLAGS = -lboost_system -lpthread -I. -std=c++11 -fPIC -Isample_libcrypto -Ilib
LINK_FLAGS = -lsample_libcrypto -Lsample_libcrypto -lcrypto

SFML = -lsfml-graphics -lsfml-window -lsfml-system

all: server

server: server.cpp ecp.cpp ias_ra.cpp network_ra.cpp service_provider.cpp crypto.cpp
	g++ -o server -g server.cpp ecp.cpp ias_ra.cpp network_ra.cpp service_provider.cpp crypto.cpp \
	./lib/Letter.cpp ./lib/image_util.cpp ./lib/processing.cpp \
	$(CPP_FLAGS) $(LINK_FLAGS) $(SFML)

clean: 
	rm server

run: server
	./server
