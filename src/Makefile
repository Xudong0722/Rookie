
server:
	g++ util.cpp client.cpp -o client -std=c++17 && \
	g++ util.cpp server.cpp  Epoll.cpp InetAddr.cpp Socket.cpp Channel.cpp -g -o server -std=c++17

clean:
	rm server && rm client  && rm *.dSYM