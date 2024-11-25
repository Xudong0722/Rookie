# Rookie
A simple cpp sever.

# File directory

Socket.h : 文件描述符类，定义了对fd的各种操作，如bind，listen，accept...

InetAddr.h : 地址信息类，存储ip地址，端口

Acceptor.h

Channel.h

Epoll.h

EventLoop.h

Sever.h

ThreadPool.h

util.h


# function

sever会初始化一个事件循环器，事件循环中会监听各种sokcet事件如新的连接，数据接收等

