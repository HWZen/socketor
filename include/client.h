#pragma once
#include "socketor.h"
#include <string>
#include <memory>



namespace mysock
{
    class Client : public socketor
    {
    public:
        explicit Client(const char *server_address = SERVER_ADDRESS, int port = DEFAULT_PORT);

        // 连接至服务器
        [[nodiscard]]int Connect2Server();

        void Send(const std::string &str) const override
        {
            socketor::Send(str);
        }


        void rawSend(const void *str, size_t len)
        {
            socketor::Send(str, len);
        }

        void CloseConnect();

        ~Client() override;

    private:
#ifdef I_OS_WIN
        WSADATA wsaData{};
#endif
        // 服务器真实地址
        std::string server_address;

        int server_port;

        std::shared_ptr<bool> hasConnected;

    public:
        //定义程序中使用的常
        constexpr static char const * const SERVER_ADDRESS = "127.0.0.1"; //服务器端IP地址
        static const int DEFAULT_PORT = 5150;
    };
} // namespace mysock
