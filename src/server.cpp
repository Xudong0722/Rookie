/*
 * @Author: Xudong0722
 * @Date: 2024-09-01 12:57:41
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-06 00:06:08
 */
#include <sys/socket.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include "util.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void set_non_blocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create failed.");

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1, "socket bind error."); // 将socket地址与文件描述符绑定

    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error."); // 监听这个socket端口

    int epfd = kqueue();
    printf("epoll fd:%d\n", epfd);
    errif(epfd == -1, "epoll create failed.");

    struct kevent active_events[MAX_EVENTS], ev;
    bzero(active_events, sizeof(active_events));

    bzero(&ev, sizeof(ev));
    set_non_blocking(sockfd);
    EV_SET(&ev, sockfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, (void *)(intptr_t)sockfd);
    int r = kevent(epfd, &ev, 1, NULL, 0, NULL);
    errif(r, "kevent failed.");

    // struct sockaddr_in client_addr;
    // socklen_t client_addr_len = sizeof(client_addr);
    // bzero(&client_addr, sizeof(client_addr));

    // int client_sockfd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_len); // 第三个参数是客户端socket长度
    // errif(client_sockfd == -1, "socket accept error.");

    // printf("new client fd %d! IP: %s Port: %d\n", client_sockfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // while (true)
    // {
    //     char buf[1024];
    //     bzero(&buf, sizeof(buf));
    //     ssize_t read_bytes = read(client_sockfd, buf, sizeof(buf));

    //     if (read_bytes > 0)
    //     {
    //         printf("message from client fd: %d, message: %s\n", client_sockfd, buf);
    //         write(client_sockfd, buf, sizeof(buf));
    //     }
    //     else if (read_bytes == 0)
    //     {
    //         printf("client fd %d disconnected.", client_sockfd);
    //         close(client_sockfd);
    //         break;
    //     }
    //     else if (read_bytes == -1)
    //     {
    //         close(client_sockfd);
    //         errif(true, "client read error.");
    //     }
    // }

    while (true)
    {
        struct timespec timeout;
        timeout.tv_sec = 10;
        timeout.tv_nsec = 0;
        int n = kevent(epfd, NULL, 0, active_events, MAX_EVENTS, &timeout);
        printf("active events: %d\n", n);
        for (int i = 0; i < n; ++i)
        {
            int fd = (int)(intptr_t)active_events[i].udata;
            int event = active_events[i].filter;
            printf("event: %d, fd:%d\n", event, fd);
            if (event == EVFILT_READ)
            {
                if (sockfd == fd)
                {
                    // accept
                    struct sockaddr_in new_client_addr;
                    bzero(&new_client_addr, sizeof(new_client_addr));
                    socklen_t new_client_addr_len = sizeof(new_client_addr);

                    int new_client_sockfd = accept(sockfd, (sockaddr *)&new_client_addr, &new_client_addr_len);
                    errif(new_client_sockfd == -1, "socket accept failed.");
                    printf("new client fd %d! IP: %s Port: %d\n", new_client_sockfd, inet_ntoa(new_client_addr.sin_addr), ntohs(new_client_addr.sin_port));

                    bzero(&ev, sizeof(ev));
                    EV_SET(&ev, new_client_sockfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, (void *)(intptr_t)new_client_sockfd);
                    int r = kevent(epfd, &ev, 1, NULL, 0, NULL);
                    errif(r, "kevent failed.");
                }
                else
                {
                    // read event
                    char buf[READ_BUFFER];
                    while (true)
                    {
                        bzero(&buf, sizeof(buf));
                        ssize_t bytes_read = read(fd, buf, sizeof(buf));

                        if (bytes_read > 0)
                        {
                            printf("message from client fd: %d, message: %s\n", fd, buf);
                            write(fd, buf, sizeof(buf));
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
                            printf("EOF, client fd %d disconnected\n", fd);
                            close(fd);
                            break;
                        }
                    }
                }
            }
            else
            {
                printf("something else happened.");
            }
        }
    }
    close(sockfd);
    return 0;
}