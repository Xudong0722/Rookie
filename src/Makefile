
server:
	g++ util.cpp client.cpp -o client && \
	g++ util.cpp server.cpp  Epoll.cpp InetAddr.cpp Socket.cpp -o server

clean:
	rm server && rm rm client