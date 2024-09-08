/*
 * @Author: Xudong0722
 * @Date: 2024-09-01 12:57:41
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-08 23:02:55
 */
#include "Socket.h"
#include "InetAddr.h"
#include "Epoll.h"
#include "util.h"
#include <unistd.h>

#define READ_BUFFER 1024

void handle_read_event(int sockfd)
{
    // read event
    char buf[READ_BUFFER];
    while (true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));

        if (bytes_read > 0)
        {
            printf("message from client fd: %d, message: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if (bytes_read == -1 && errno == EINTR)
        {
            printf("continue reading.");
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            printf("finish reading once, errno: %d\n", errno);
            break;
        }
        else if (bytes_read == 0)
        {
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
    }
}

int main()
{
    Socket *server_sock = new Socket;
    InetAddr *server_addr = new InetAddr("127.0.0.1", 8888);

    server_sock->bind(server_addr);
    server_sock->listen();

    Epoll *epfd = new Epoll;
    server_sock->set_non_blocking();

    epfd->add_fd(server_sock->get_fd(), EV_ADD | EV_ENABLE);

    while (true)
    {
        auto active_events = epfd->wait(30);
        int n = active_events.size();
        printf("active events: %d\n", n);
        for (int i = 0; i < n; ++i)
        {
            int fd = (int)(intptr_t)active_events[i].udata;
            int event = active_events[i].filter;
            printf("event: %d, fd:%d\n", event, fd);
            if (event == EVFILT_READ)
            {
                if (server_sock->get_fd() == fd)
                {
                    InetAddr *new_client_addr = new InetAddr;
                    Socket *new_client_sock = new Socket(server_sock->accept(new_client_addr));

                    printf("new client fd %d! IP: %s Port: %d\n", new_client_sock->get_fd(), inet_ntoa(new_client_addr->addr_info_.sin_addr), ntohs(new_client_addr->addr_info_.sin_port));
                    epfd->add_fd(new_client_sock->get_fd(), EV_ADD | EV_ENABLE);
                }
                else
                {
                    handle_read_event(fd);
                }
            }
            else
            {
                printf("something else happened.");
            }
        }
    }

    return 0;
}