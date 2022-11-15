//
// Created by HWZ on 2021/1/19.
//

#ifndef NBUT_BEST_SQL_SOCKETOR_H
#define NBUT_BEST_SQL_SOCKETOR_H

#include <string>
#include "osplatformutil.h"

#ifdef I_OS_WIN

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#endif

#define MAX_SOCKET_SIZE 0x10000

#ifdef I_OS_LINUX

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
#define closesocket(x) close(x)
#define SOCKET_ERROR -1

#define TRUE 1

#endif // I_OS_LINUX


#ifdef __GNUC__


#endif

namespace mysock
{
    enum flag{
        SUCESS,
        LISTEN_SUCESS = SUCESS,
        BIND_FAIL,
        LISTEN_FAIL,
        CONNECT_FAIL,
        EMPTY_OBJ,
        NO_LISTENTED,
        BYTE_FAIL,
        WSA_ERROR,
        GET_HOST_NAME_FAIL
    };

    class socketor
    {
    protected:
        SOCKADDR_IN Socket_info{};
        std::string Address;
        int Port{};

        SOCKET Socket{};
    public:
        socketor() = default;

        socketor(SOCKET target_socket, SOCKADDR_IN socket_info);

        virtual ~socketor() = default;

        // 发送函数
        virtual void Send(const void *str, size_t len) const;
        virtual void Send(const std::string &str) const;

        // 接收函数
        virtual int receive(void *buf, size_t len) const;
        virtual std::string receive() const;


        // Socket地址
        std::string address() const
        {
            return Address;
        };

        // Socket端口
        int port() const
        {
            return Port;
        };

        SOCKET getRawSocket(){return Socket;}
    };


} // namespace mysock

// SOCKET的封装



#endif //NBUT_BEST_SQL_SOCKETOR_H
