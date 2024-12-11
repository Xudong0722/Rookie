# Rookie
A simple cpp sever.

# File directory

Socket.h : 
文件描述符类，定义了对fd的各种操作，如bind，listen，accept...实际上是对socket的封装。

InetAddr.h : 地址信息类，存储ip地址，端口。实际上是对sockaddr_in的封装。

Acceptor.h

Channel.h

Epoll.h

EventLoop.h

Sever.h
服务端类，基于IO复用，主从Reactor模式。
One loop per thread， 每个线程都有一个事件循环，主线程也即主Reactor使用IO复用来接受客户端连接，然后分配给sub reactor。

ThreadPool.h
线程池类

util.h
用来存放一些工具方法

TODO
1. 使用cmake来重新组织文件，工程化
2. 使用智能指针
3. EventLoop不可拷贝，生命周期和其所属线程一样长，所以不必是放在heap上
4. 构建日志库

