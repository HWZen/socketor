//
// Created by HWZ on 2021/1/19.
//

#include "socketor.h"
#include <cstring>

#ifdef I_OS_LINUX

#include <time.h>

void Sleep(int ms)
{
    struct timeval delay;
    delay.tv_sec = 0;
    delay.tv_usec = ms * 1000; // 20 ms
    select(0, NULL, NULL, NULL, &delay);
}

#endif // I_OS_LINUX

#ifdef I_OS_WIN

#include <stdexcept>

#endif // I_OS_WIN

namespace mysock
{
    socketor::socketor()
    {
#ifdef I_OS_WIN
        int err = 0;
        ++init_count;
        if (init_count == 1)
        {
            WORD wVersionRequested = 0;

            wVersionRequested = MAKEWORD(2, 2);

            err = WSAStartup(wVersionRequested, &wsaData);
            if(err != 0){
                throw std::runtime_error("WSAStartup failed");
            }

            if (LOBYTE(wsaData.wVersion) != 2 ||
                HIBYTE(wsaData.wVersion) != 2)
            {
                WSACleanup();
                throw std::runtime_error("byte alignment failed");
            }
        }
#endif // I_OS_WIN

    }

    socketor::socketor(SOCKET target_socket, SOCKADDR_IN socket_info)
    {
        Socket = target_socket;
        Socket_info.sin_family = socket_info.sin_family;
        Socket_info.sin_port = socket_info.sin_port;
        Socket_info.sin_addr = socket_info.sin_addr;
        Address = inet_ntoa(socket_info.sin_addr);
        Port = ntohs(socket_info.sin_port);
    }

    void socketor::Send(const void *dataBuf, size_t len) const
    {
        send(Socket, (char *)dataBuf, len, 0);
    }
    void socketor::Send(const std::string &str) const
    {
        send(Socket, str.c_str(), str.size(), 0);
    }

    int64_t socketor::receive(void *buf, size_t len) const
    {
        return recv(Socket, (char *)buf, len, 0);
    }
    std::string socketor::receive() const
    {
        char buf[MAX_SOCKET_SIZE];
        auto ret = recv(Socket, buf, MAX_SOCKET_SIZE, 0);
        if (ret <= 0)
            return {};
        buf[ret] = '\0';
        return buf;
    }

    socketor::~socketor()
    {
#ifdef I_OS_WIN
        --init_count;
        if (init_count == 0)
            WSACleanup();
#endif // I_OS_WIN
    }

} // namespace mysock

