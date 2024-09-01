/*
 * @Author: Xudong0722
 * @Date: 2024-09-01 12:57:41
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-01 14:09:02
 */
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create failed.");

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)), "socket bind error."); // 将socket地址与文件描述符绑定

    errif(listen(sockfd, SOMAXCONN), "socket listen error."); // 监听这个socket端口

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));

    int client_sockfd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_len); // 第三个参数是客户端socket长度
    errif(client_sockfd == -1, "socket accept error.");

    printf("new client fd %d! IP: %s Port: %d\n", client_sockfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    while (true)
    {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(client_sockfd, buf, sizeof(buf));

        if (read_bytes > 0)
        {
            printf("message from client fd: %d, message: %s\n", client_sockfd, buf);
            write(client_sockfd, buf, sizeof(buf));
        }
        else if (read_bytes == 0)
        {
            printf("client fd %d disconnected.", client_sockfd);
            close(client_sockfd);
            break;
        }
        else if (read_bytes == -1)
        {
            close(client_sockfd);
            errif(true, "client read error.");
        }
    }
    close(sockfd);
    return 0;
}