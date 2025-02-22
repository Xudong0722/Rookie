总体

我们先看看整体的流程：
所谓Linux server，一定是提供服务的，那么就需要有对应的client与server建立通信再根据业务需求做不同的处理。

Server
在创建一个server之后，我们使用闭包的方式实现server的功能，可以看到，下面的代码就是将新的连接conn发过来的消息原封不动的传了回去，也就是我们所有的echo服务！
  EventLoop *loop = new EventLoop();
  TcpServer *server = new TcpServer(loop);  //每个server内嵌一个事件循环，由事件驱动

  server->on_connect([](Connection *conn) {
    conn->read();
    if (conn->get_state() == Connection::STATE::CLOSED) {
      conn->close();
      return;
    }
    std::cout << "Message from client " << conn->get_socket()->get_fd() << ": " << conn->read_buf() << std::endl;
    conn->set_send_buf(conn->read_buf());  //将client发送的数据写到buf里面
    conn->write();
  });
  loop->loop();

我们知道了server提供什么样的服务之后，看看EventLoop和TcpServer是什么。
在那之前，我们先了解几个最基础的概念。

Socket
InetAddr是sockaddr_in的封装，用于存储ip地址和端口号。而sockaddr是一个通用的地址结构体，可以用于不同类型的地址（如IPv4和IPv6），而sockaddr_in专门用于IPv4地址。

Socket，套接字，是网络主机通信中不同进程之间通信的端点，是一层抽象。一个套接字就是网络上进程通信的一端，提供了应用层进程利用网络协议交换数据的机制。在服务端和客户端建立连接之后，就可以通信了，他们从哪里获取对端发送/接收的消息呢，就是通过文件描述符fd，在创立连接时，操作系统会为其分配一个fd，后续我们就可以通过监听这个文件描述符来处理通信数据！
为了表示和区分已经打开的文件，UNIX/Linux 会给每个文件分配一个 ID，这个 ID 就是一个整数，被称为文件描述符（File Descriptor）。例如：
● 通常用 0 来表示标准输入文件（stdin），它对应的硬件设备就是键盘；
● 通常用 1 来表示标准输出文件（stdout），它对应的硬件设备就是显示器。

UNIX/Linux 程序在执行任何形式的 I/O 操作时，都是在读取或者写入一个文件描述符。一个文件描述符只是一个和打开的文件相关联的整数，它的背后可能是一个硬盘上的普通文件、FIFO、管道、终端、键盘、显示器，甚至是一个网络连接。
网络连接也是一个文件，它也有文件描述符！
我们可以通过 socket() 函数来创建一个网络连接，或者说打开一个网络文件，socket() 的返回值就是文件描述符。有了文件描述符，我们就可以使用普通的文件操作函数来传输数据了，例如：
● 用 read() 读取从远程计算机传来的数据；
● 用 write() 向远程计算机写入数据。

关于socket有几个关键的函数：
/*
       When a socket is created with socket(2), it exists in a name space
       (address family) but has no address assigned to it.  bind()
       assigns the address specified by addr to the socket referred to by
       the file descriptor sockfd.  addrlen specifies the size, in bytes,
       of the address structure pointed to by addr.  Traditionally, this
       operation is called “assigning a name to a socket”.
*/

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);


/*
       listen() marks the socket referred to by sockfd as a passive
       socket, that is, as a socket that will be used to accept incoming
       connection requests using accept(2).

       The sockfd argument is a file descriptor that refers to a socket
       of type SOCK_STREAM or SOCK_SEQPACKET.

       The backlog argument defines the maximum length to which the queue
       of pending connections for sockfd may grow.  If a connection
       request arrives when the queue is full, the client may receive an
       error with an indication of ECONNREFUSED or, if the underlying
       protocol supports retransmission, the request may be ignored so
       that a later reattempt at connection succeeds.
*/
int listen(int sockfd, int backlog);


/*
        The connect() system call connects the socket referred to by the
       file descriptor sockfd to the address specified by addr.  The
       addrlen argument specifies the size of addr.  The format of the
       address in addr is determined by the address space of the socket
       sockfd; see socket(2) for further details.

       If the socket sockfd is of type SOCK_DGRAM, then addr is the
       address to which datagrams are sent by default, and the only
       address from which datagrams are received.  If the socket is of
       type SOCK_STREAM or SOCK_SEQPACKET, this call attempts to make a
       connection to the socket that is bound to the address specified by
       addr.
*/
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);



/*
       The accept() system call is used with connection-based socket
       types (SOCK_STREAM, SOCK_SEQPACKET).  It extracts the first
       connection request on the queue of pending connections for the
       listening socket, sockfd, creates a new connected socket, and
       returns a new file descriptor referring to that socket.  The newly
       created socket is not in the listening state.  The original socket
       sockfd is unaffected by this call.
*/

int accept(int sockfd, struct sockaddr *_Nullable restrict addr,
                  socklen_t *_Nullable restrict addrlen);


/*
       fcntl() performs one of the operations described below on the open
       file descriptor fd.  The operation is determined by op.

       fcntl() can take an optional third argument.  Whether or not this
       argument is required is determined by op.  The required argument
       type is indicated in parentheses after each op name (in most
       cases, the required type is int, and we identify the argument
       using the name arg), or void is specified if the argument is not
       required.

*/
int fcntl(int fd, int op, ... /* arg */ );

 还有两个函数没有说明， set_non_blocking和is_non_blocking()。 我们需要搞清楚阻塞和非阻塞。socket默认是阻塞的，如果我们调用accpet准备接收新的连接，但是并没有连接，阻塞式socket就会让进程挂起一直等到新的连接，而非阻塞式socket就会返回错误。
       If no pending connections are present on the queue, and the socket
       is not marked as nonblocking, accept() blocks the caller until a
       connection is present.  If the socket is marked nonblocking and no
       pending connections are present on the queue, accept() fails with
       the error EAGAIN or EWOULDBLOCK.

class InetAddr;

class Socket
  : protected noncopymoveable {
 public:
  Socket();
  explicit Socket(int _fd);
  ~Socket();

 public:
  void bind(InetAddr *);
  void listen();
  void set_non_blocking();
  bool is_non_blocking();
  int accept(InetAddr *);
  void connect(InetAddr *);
  void connect(const char *ip, uint16_t port);
  int get_fd();

 private:
  int fd_;
};

Epoll
了解完socket以后，我们一个简单的echo服务器：
Server:
import socket
import threading

def handle_client(conn):
    while True:
        data = conn.recv(1024)  # 阻塞点
        if not data:
            break
        print(data)
        conn.send(b"Echo: " + data)
    conn.close()

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(('127.0.0.1', 8888))
server.listen(5)

while True:
    conn, addr = server.accept()  # 阻塞点
    # 为每个客户端创建新线程
    threading.Thread(target=handle_client, args=(conn,)).start()

Client:
import socket
import threading

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('127.0.0.1', 8888))
while True:
    inp = input()
    binp = bytes(inp, encoding='utf8')
    print(binp)
    client.send(binp)
    data = client.recv(1024)
    print(data)

两端代码非常简单，就是client发送一个数据，server端收到之后原封不动的再发送回来。我们看到server端是怎么实现的，一个死循环，等待新的连接，给每个新的连接都分配了一个线程去处理。容易想到，这非常容易耗尽系统资源，线程数不能无休止的增加。同时，具体到某一个线程来说，它只负责一个连接，如果没有数据过来的话，他也会一直阻塞等待数据到来，这段时间也是非常浪费的，我们只能等待。所以linux网络编程中，提供了几个方法帮助我们应对这些场景。常见的是这三个接口：poll，select，epoll。在我们的代码中使用epoll，也是目前比较主流的接口。它们的主要思想都是IO多路复用，一个线程可以服务多个IO连接，也就是说一个线程可以监听并且处理多个IO请求。
import socket
import select

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind(('127.0.0.1', 8888))
server.listen(5)
server.setblocking(False)

epoll = select.epoll()
epoll.register(server.fileno(), select.EPOLLIN)

connections = {}
requests = {}
responses = {}

while True:
    events = epoll.poll(1)  # 核心：阻塞直到事件发生
    for fd, event in events:
        if fd == server.fileno():
            # 处理新连接
            conn, addr = server.accept()
            conn.setblocking(False)
            client_fd = conn.fileno()
            epoll.register(client_fd, select.EPOLLIN)
            connections[client_fd] = conn
        elif event & select.EPOLLIN:
            # 处理可读事件
            data = connections[fd].recv(1024)
            if data:
                requests[fd] = data
                epoll.modify(fd, select.EPOLLOUT)  # 改为监听写事件
            else:
                epoll.unregister(fd)
                connections[fd].close()
        elif event & select.EPOLLOUT:
            # 处理可写事件
            connections[fd].send(b"Echo: " + requests[fd])
            epoll.modify(fd, select.EPOLLIN)  # 改回监听读事件
使用epoll，我们就可以使用单线程监听并处理多个client的连接，且epoll的效率较高，它在内核中维护了一个红黑树，将我们注册的fd放在红黑树中，并增加了一个中断函数，一旦这个fd上有我们关心的事件发生，就在一个链表中加入此事件，这样我们在调用epoll获取发生的事件时内核只需要关注链表中的事件即可，而不需要遍历所有的fd。
Channel
在上面一小节，我们通过epoll就可以实现在一个线程中监听多个IO连接，具体一些是接收到新的连接后将新的fd放入epoll中监听后续这个fd的其他事件。但是直接通过fd来处理所有的事件的话，事务监听和业务逻辑的代码会混在一起，耦合度较高，我们可以针对fd封装一个类，事件的回调也可以定制写在这个类中，这就是Channel的作用。并且，可以提供扩展性，例如不同功能的server，处理fd的逻辑不一样，我们可以通过继承Channel来实现不同的功能！

class Epoll;
class EventLoop;

class Channel
: protected noncopymoveable {
public:
Channel(EventLoop *event_loop, int fd);
~Channel();

void enable_reading();
void handle_event();

int get_fd();
uint32_t get_listen_events() const;

uint32_t get_ready_events() const;
void set_ready_events(uint32_t ready);

bool get_in_epoll() const;
void set_in_epoll(bool is_in_epoll);

void use_ET();
void set_read_callback(const std::function<void()>& cb);

private:
EventLoop *event_loop_;           //每个channel都属于一个sub reactor
int fd_{-1};                      //此channel负责的fd

uint32_t listen_events_{0};       //当前这个fd感兴趣的事件（需要监听的事件）
uint32_t ready_events_{0};        //当前这个fd触发的事件
bool is_in_epoll_{false};         //当前这个channel是不是在epoll的监听中
std::function<void()> read_cb_;   //业务方注入，读事件回调
std::function<void()> write_cb_;  //业务方注入，写事件回调
};
EventLoop
class Epoll;
class Channel;
class ThreadPool;

class EventLoop
  : protected noncopymoveable {
 public:
  EventLoop();
  ~EventLoop();

  void loop();
  void update_channel(Channel *channel);

 private:
  Epoll *ep_;
  bool quit_{false};
};




2.Elog
基于Leveldb的log系统，做一些改进！

API：
1. snprintf() 是一个 C 语言标准库函数，用于格式化输出字符串，并将结果写入到指定的缓冲区，与 sprintf() 不同的是，snprintf() 会限制输出的字符数，避免缓冲区溢出。C 库函数 int snprintf(char *str, size_t size, const char *format, ...) 设将可变参数(...)按照 format 格式化成字符串，并将字符串复制到 str 中，size 为要写入的字符的最大数目，超过 size 会被截断，最多写入 size-1 个字符。
//https://man7.org/linux/man-pages/man3/snprintf.3p.html
int snprintf(char *restrict s, size_t n,
           const char *restrict format, ...);


2. 根据format的格式将ap中的数据写入缓冲区，缓冲区大小为n，默认在最后添加结尾符，所以最大可写入字符数量为n-1，如果超过的话，仅返回写入成功的字符数量，并不会报错
//https://man7.org/linux/man-pages/man3/vsnprintf.3p.html
int vsnprintf(char *restrict s, size_t n,
           const char *restrict format, va_list ap);
