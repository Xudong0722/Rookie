
server:
	g++ src/util.cpp client.cpp -o client -std=c++17 && \
	g++ src/util.cpp src/Server.cpp  \
	src/Epoll.cpp src/InetAddr.cpp \
	src/Socket.cpp src/Channel.cpp \
	src/EventLoop.cpp src/Acceptor.cpp \
	src/Connection.cpp \
	server.cpp -o server -std=c++17

clean:
	rm server && rm client