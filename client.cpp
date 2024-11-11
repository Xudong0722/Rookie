/*
 * @Author: Xudong0722
 * @Date: 2024-09-01 12:57:55
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-11-12 01:16:09
 */
#include "src/util.h"
#include "src/Buffer.h"
#include "src/Socket.h"
#include "src/InetAddr.h"
#include <iostream>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
int main()
{
    // int sockfd = socket(AF_INET, SOCK_STREAM, 0); // IPV4, 数据流格式, 协议类型-0表示自动推导
    // errif(sockfd == -1, "socket create failed.");

    // struct sockaddr_in server_addr;
    // bzero(&server_addr, sizeof(server_addr));

    // server_addr.sin_family = AF_INET;                     // 设置地址族
    // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 设置IP端口
    // server_addr.sin_port = htons(8888);                   // 设置端口

    // errif(connect(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)), "connect server failed.");

    Socket *sock = new Socket();
    InetAddr *server_addr = new InetAddr("127.0.0.1", 1234);
    sock->connect(server_addr);

    int sockfd = sock->get_fd();

    Buffer *send_buffer = new Buffer();
    Buffer *read_buffer = new Buffer();
    while (true)
    {
        send_buffer->getline();
        ssize_t write_bytes = write(sockfd, send_buffer->c_str(), send_buffer->size());

        if (write_bytes == -1)
        {
            printf("socket alread disconnected, can't write anymore.");
            break;
        }

        int already_read{0};
        char buf[1024];
        while (true)
        {
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));

            if (read_bytes > 0)
            {
                read_buffer->append(buf, read_bytes);
                already_read += read_bytes;
                printf("message from server: %s\n", buf);
            }
            else if (read_bytes == 0)
            {
                printf("server socket disconnected.");
                exit(EXIT_SUCCESS);
            }
            else if (read_bytes == -1)
            {
                close(sockfd);
                errif(true, "socket read error.");
            }

            if (already_read >= send_buffer->size())
            {
                printf("message from server: %s\n", read_buffer->c_str());
                break;
            }
        }
        read_buffer->clear();
    }

    delete server_addr;
    delete sock;
    return 0;
}