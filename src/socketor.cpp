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
    socketor::socketor()
    {
#ifdef I_OS_WIN // wsaStartup
        int err = 0;
        ++init_count;
        if (init_count == 1)
            WSASTARTUP();
#endif // I_OS_WIN

        hasConnected = std::make_shared<std::atomic_bool>(false);

    }

    socketor::socketor(SOCKET target_socket, SOCKADDR_IN socket_info)
    {
        Socket = target_socket;
        Socket_info.sin_family = socket_info.sin_family;
        Socket_info.sin_port = socket_info.sin_port;
        Socket_info.sin_addr = socket_info.sin_addr;
        Address = inet_ntoa(socket_info.sin_addr);
        Port = ntohs(socket_info.sin_port);
        hasConnected = std::make_shared<std::atomic_bool>(false);
    }


    socketor::~socketor()
    {
#ifdef I_OS_WIN
        --init_count;
        if (init_count == 0)
            WSACLEANUP();
#endif // I_OS_WIN
    }

    socketor::socketor(const socketor& other)
    {
        Address = other.Address;
        Port = other.Port;
        Socket = other.Socket;
        Socket_info = other.Socket_info;
        hasConnected = other.hasConnected;
#ifdef I_OS_WIN
        ++init_count;
#endif // I_OS_WIN
    }

    socketor::socketor(socketor&& other) noexcept
    {
        Address = other.Address;
        Port = other.Port;
        Socket = other.Socket;
        Socket_info = other.Socket_info;
        hasConnected = other.hasConnected;
#ifdef I_OS_WIN
        ++init_count;
#endif // I_OS_WIN
    }

    socketor& socketor::operator=(const socketor& other)
    {
        if(this == &other)
            return *this;
        if(hasConnected->load())
            throw std::runtime_error("try to assign a socket that has connected");
        Address = other.Address;
        Port = other.Port;
        Socket = other.Socket;
        Socket_info = other.Socket_info;
        hasConnected = other.hasConnected;
        return *this;
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

#ifdef I_OS_WIN
    void socketor::WSACLEANUP()
    {
        WSACleanup();
    }

    void socketor::WSASTARTUP()
    {
        WORD wVersionRequested = 0;

        wVersionRequested = MAKEWORD(2, 2);

        auto err = WSAStartup(wVersionRequested, &wsaData);
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


} // namespace mysock

