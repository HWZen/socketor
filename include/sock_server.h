#pragma once
#include "socketor.h"
#include <vector>
#include <thread>
#include <unordered_map>
#include <memory>

#define DEFAULT_PORT 5150

#ifdef I_OS_LINUX
#define SOCK_DEC
#endif


namespace mysock
{

    void Accept_call_back(socketor s);
    class server : public socketor
    {
    public:
        explicit server(int Port = DEFAULT_PORT);

        server(const server &s) = default;

        server(server &&s) noexcept ;


        // 开启监听函数
        int Listen() noexcept(false);

        // 受理连接并调用接收函数
        int Accept(void(* call_back)(socketor));

        void Send(const std::string &str) override
        {
            socketor::Send(str);
        }

        void rawSend(const void *str, size_t len)
        {
            socketor::Send(str, len);
        }

        void close_connect();
        static void close_connect(socketor s);

        ~server();

    private:
#ifdef I_OS_WIN
        WSADATA wsaData{};
#endif
        std::shared_ptr<bool> hasListened;



    };
} // namespace mysock

