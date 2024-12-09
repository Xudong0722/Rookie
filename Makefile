
server:
	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp \
	src/InetAddr.cpp client.cpp -o client -std=c++17 && \
	g++ src/util.cpp src/Server.cpp  \
	src/Epoll.cpp src/InetAddr.cpp \
	src/Socket.cpp src/Channel.cpp \
	src/EventLoop.cpp src/Acceptor.cpp \
	src/Buffer.cpp \
	src/Connection.cpp \
	src/ThreadPool.cpp \
	server.cpp -o server -std=c++17 -pthread

clean:
	rm server && rm client

threadTest:
	g++ src/ThreadPool.cpp  ThreadPoolTest.cpp -o ThreadPoolTest -std=c++17