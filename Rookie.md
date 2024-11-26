*在学习多线程和网络编程时，总有些名词出现的频率很高，所以在此文档中记录下这些名词的基本含义，或者不同书籍中同一名词的不同解释*


**IO操作**
 Input/Output，通常指数据在存储器或其他设备之间的的输入和输出，直观的例子就是访问磁盘中的数据。

**IO密集**
 IO-Bound，程序运行的效率瓶颈在IO操作上，如果IO设备更快，那么程序的效率就可以更快，直观的例子就是磁盘IO和网络IO

**CPU密集**
 CPU-Bound，程序运行的效率瓶颈在CPU计算上，如果CPU更快，那么程序的效率就可以更快，直观的例子是subset problem(给定一个整数集合，问是否存在一个非空子集，使得子集中的数字和为某个特定数值，这是一个NP完全问题，时间复杂度为O(2^n))，一个很小的n也会导致CPU一直在计算

**IO多路复用(IO Multiplexing)**
1. 多个连接共享一个阻塞对象，应用程序只会在一个阻塞对象上等待

2. I/O multiplexing is the capability to tell the kernel that we want to be notified if one or more I/O conditions are ready, like input is ready to be read, or descriptor is capable of taking more output.（https://medium.com/@rrakshith007/io-multiplexing-and-its-advantages-8c75584079d1）

3.这是一个能力，可以在单线程中监听多个IO请求，如select，poll，epoll，在一个线程中来检查多个socket的就绪状态


**流**
1. 可以进行IO操作的内核对象
2. 传输媒介可以是文件，管道，文件描述符
3. 数据的入口是通过文件描述符（fd）描述