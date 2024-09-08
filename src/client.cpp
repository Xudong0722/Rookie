/*
 * @Author: Xudong0722
 * @Date: 2024-09-01 12:57:55
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-03 23:38:30
 */
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

#define BUFFER_SIZE 1024
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // IPV4, 数据流格式, 协议类型-0表示自动推导
    errif(sockfd == -1, "socket create failed.");

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;                     // 设置地址族
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 设置IP端口
    server_addr.sin_port = htons(8888);                   // 设置端口

    errif(connect(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)), "connect server failed.");

    while (true)
    {
        char buf[BUFFER_SIZE];
        bzero(&buf, sizeof(buf));

        scanf("%s", buf);
        ssize_t write_types = write(sockfd, buf, sizeof(buf));

        if (write_types == -1)
        {
            printf("socket alread disconnected, can't write anymore.");
            break;
        }

        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));

        if (read_bytes > 0)
        {
            printf("message from server: %s\n", buf);
        }
        else if (read_bytes == 0)
        {
            printf("server socket disconnected.");
            break;
        }
        else if (read_bytes == -1)
        {
            close(sockfd);
            errif(true, "socket read error.");
        }
    }

    close(sockfd);
    return 0;
}