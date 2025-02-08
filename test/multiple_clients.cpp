/*
 * @Author: Xudong0722 
 * @Date: 2024-12-29 13:16:32 
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-12-29 13:37:28
 */

#include <unistd.h>
#include <cstring>
#include <functional>
#include <iostream>

#include "Connection.h"
#include "Socket.h"
#include "ThreadPool.h"

void one_client(int msgs, int wait)
{
    Socket* sock = new Socket();
    sock->connect("127.0.0.1", 8888);

    Connection* conn = new Connection(nullptr, sock);
    sleep(wait);
    int count{0};
    while(count < msgs){
        conn->set_send_buf("I am a client");
        conn->write();    
        if(conn->get_state() == Connection::STATE::CLOSED){
            conn->close();
            break;
        }
        conn->read();
        std::cout << "msg count:" << count++ << ": " << conn->read_buf() << std::endl;
    }
    delete conn;
}

int main(int argc, char *argv[])
{
    int threads{100};
    int msgs{100};
    int wait{0};
    int o{-1};
    const char* opt_string = "t:m:w:";
    while((o = getopt(argc, argv, opt_string)) != -1){
        switch(o){
            case 't':
            {
            threads = std::stoi(optarg);
            break;
            }

            case 'm':
            {
            msgs = std::stoi(optarg);
            break;
            }
            case 'w':
            {
            wait = std::stoi(optarg);
            break;
            }
            case '?':
            {
            printf("error optopt: %c\n", optopt);
            printf("error opterr: %d\n", opterr);
            break;
            }
            default:
            break;
        }
    }

    ThreadPool * poll = new ThreadPool(threads);
    std::function<void()> func = std::bind(one_client, msgs, wait);
    for(int i = 0; i < threads; ++i){
        poll->add(func);
    }
    delete poll;
    return 0;
}