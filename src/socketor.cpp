//
// Created by HWZ on 2021/1/19.
//

#include "socketor.h"
#include <stdexcept>
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

    socketor::socketor(SOCKET socket_fd, SOCKADDR_IN socket_info)
    {
        Socket = socket_fd;
        Socket_info.sin_family = socket_info.sin_family;
        Socket_info.sin_port = socket_info.sin_port;
        Socket_info.sin_addr = socket_info.sin_addr;
        Address = inet_ntoa(socket_info.sin_addr);
        Port = ntohs(socket_info.sin_port);
    }

    int64_t socketor::Send(const void *dataBuf, size_t len) const
    {
        return send(Socket, (char *)dataBuf, len, 0);
    }

    int64_t socketor::Send(const std::string &str) const
    {
        return send(Socket, str.c_str(), str.size(), 0);
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

    int socketor::setSendTimeout(int timeout) {
        {
#ifdef I_OS_LINUX
            struct timeval tv;
            tv.tv_sec = timeout / 1000;
            tv.tv_usec = (timeout % 1000) * 1000;
            return setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(struct timeval));
#endif // I_OS_LINUX

#ifdef I_OS_WIN
            return setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
#endif // I_OS_WIN
        }
    }

    int socketor::setRecvTimeout(int timeout) {
        {
#ifdef I_OS_LINUX
            struct timeval tv;
            tv.tv_sec = timeout / 1000;
            tv.tv_usec = (timeout % 1000) * 1000;
            return setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));
#endif // I_OS_LINUX

#ifdef I_OS_WIN
            return setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
#endif // I_OS_WIN

        }
    }

    socketor socketor::makeSocketor()
    {
#ifdef I_OS_WIN
        WsaInitClass::WsaInit();
#endif
        socketor sock;
        sock.Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sock.Socket_info.sin_family = AF_INET;
        sock.Socket_info.sin_port = htons(SOCKETOR_DEFAULT_PORT);
        sock.Socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
        sock.Port = SOCKETOR_DEFAULT_PORT;
        return sock;
    }
} // namespace mysock

