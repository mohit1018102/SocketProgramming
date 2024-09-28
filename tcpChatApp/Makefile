CXX=g++

all: client server

client: tcpClient.cpp
		$(CXX) -o client.out tcpClient.cpp
server: tcpServer.cpp
		$(CXX) -o server.out tcpServer.cpp


clean:
		rm -f *out