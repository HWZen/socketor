#pragma once
#include "socketor.h"
#include <string>
#include <memory>

//定义程序中使用的常量
#define SERVER_ADDRESS "127.0.0.1" //服务器端IP地址
#define DEFAULT_PORT 6800          //服务器的端口号

#ifdef I_OS_LINUX
#define SOCK_DEC
#endif

namespace mysock
{
    class Client : public socketor
    {
    public:
        explicit Client(const char *server_address = SERVER_ADDRESS, int port = DEFAULT_PORT);

        // 连接至服务器
        [[nodiscard]]int connect2server();

        void Send(const std::string &str) override
        {
            socketor::Send(str);
        }


        void rawSend(const void *str, size_t len)
        {
            socketor::Send(str, len);
        }

        void close_connect();

    private:
#ifdef I_OS_WIN
        WSADATA wsaData{};
        size_t _msgSize{};
#endif
        // 服务器真实地址
        std::string server_address;

        int server_port;

        std::shared_ptr<bool> hasConnected;
    };
} // namespace mysock
