//
// Created by HWZ on 2021/1/19.
//

#include "socketor.h"
#include <string.h>

#ifdef I_OS_LINUX

#include <time.h>

void Sleep(int ms)
{
    struct timeval delay;
    delay.tv_sec = 0;
    delay.tv_usec = ms * 1000; // 20 ms
    select(0, NULL, NULL, NULL, &delay);
}

#endif

namespace mysock
{
    socketor::socketor(SOCKET target_socket, SOCKADDR_IN socket_info)
    {
        Socket = target_socket;
        Socket_info.sin_family = socket_info.sin_family;
        Socket_info.sin_port = socket_info.sin_port;
        Socket_info.sin_addr = socket_info.sin_addr;
        Address = inet_ntoa(socket_info.sin_addr);
        Port = ntohs(socket_info.sin_port);
    }

    void socketor::Send(const void *str, size_t len)
    {
        send(Socket, (char *)str, len, 0);
    }
    void socketor::Send(const std::string &str)
    {
        send(Socket, str.c_str(), str.size(), 0);
    }

    int socketor::receive(void *buf, size_t len)
    {
        return recv(Socket, (char *)buf, len, 0);
    }
    std::string socketor::receive()
    {
        char buf[MAX_SOCKET_SIZE];
        int ret = recv(Socket, buf, MAX_SOCKET_SIZE, 0);
        if (ret <= 0)
            return "lose connect";
        buf[ret] = '\0';
        return buf;
    }

    void socketor::close_connect()
    {
        closesocket(Socket);
#ifdef I_OS_WIN
        WSACleanup();
#endif
    }

} // namespace mysock

