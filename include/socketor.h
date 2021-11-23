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

#define MAX_SOCKET_SIZE 4096

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

#define TRUE 1

#ifdef DEC
#define SOCK_DEC
#else
#define SOCK_DEC
#endif // DEC

#endif // I_OS_LINUX

#ifdef I_OS_WIN
#ifdef DEC
#define SOCK_DEC  __declspec(dllexport)
#else
#define SOCK_DEC  __declspec(dllimport)
#endif // DEC
#endif // I_OS_WIN


#ifdef __GNUC__

namespace std{
    class msvc_exception
    {
    protected:
        string describe;
    public:
        msvc_exception(string str):describe(str){}
        msvc_exception():describe("no describe"){}
        virtual const char* what(){return describe.c_str();}
    };
#define exception msvc_exception
}

#endif

namespace mysock
{
    enum flag{
        LISTEN_SUCESS,
        BIND_FAIL,
        LISTEN_FAIL
    };

    class SOCK_DEC socketor
    {
    protected:
        SOCKET Socket{};
        SOCKADDR_IN Socket_info{};
        std::string Address;
        int Port{};

    public:
        socketor() = default;

        socketor(SOCKET target_socket, SOCKADDR_IN socket_info);

        // 发送函数
        virtual void Send(const void *str, size_t len);
        virtual void Send(const std::string &str);

        // 接收函数
        virtual int receive(void *buf, size_t len = MAX_SOCKET_SIZE);
        virtual std::string receive();

        void close_connect();

        // Socket地址
        inline std::string address()
        {
            return Address;
        };

        // Socket端口
        inline int port() const
        {
            return Port;
        };
    };

    template<typename Ty>
    class exception : public std::exception
    {
    private:
        Ty error_date;
    public:
        Ty date(){return error_date;}
        const char *describe(){return exception::what();}
        explicit exception(Ty date): error_date(date), std::exception(){}
        exception(Ty date, const char *describe): error_date(date), std::exception(describe){}

    };


} // namespace mysock

// SOCKET的封装



#endif //NBUT_BEST_SQL_SOCKETOR_H
